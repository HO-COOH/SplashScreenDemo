#include <Windows.h>
#include "StartupTimer.h"
#include <optional>
#include "MainApplicationLoadingMessageQueue.h"


#include <wil/com.h>

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include "Config.h"
#include <dcomp.h>
#include "Background.h"
#include "CompositionWrapper.h"
#include <d2d1_2.h>
#include "D2D1Factory.h"
#include <dwmapi.h>
#include "LogoCompositionSurface.h"
#include "DoubleAnimationUsingKeyFrames.h"
#include "ProgressBarComposition.h"
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "dcomp.lib")

static LPSTR argv{};
static HWND syncMoveWithWindow{};
static std::optional<MainApplicationLoadingMessageQueue> messageQueue;


auto ScaleForDpi(auto value, UINT dpi)
{
	return static_cast<float>(value * dpi / 96);
};


//WIP
class SplashWindow
{
	HWND m_hwnd;
	std::optional<CompositionWrapper> m_compositionWrapper;

	Background background{ nullptr };
	DoubleAnimationUsingKeyFrames opacityAnimation{ nullptr };
	winrt::Windows::UI::Composition::SpriteVisual logoSprite{ nullptr };
	ProgressBarComposition progressBar{ nullptr };

	constexpr static auto className = L"SplashWindowClass";
	static void registerClassIfNeeded()
	{
		static bool registered = false;
		
		if (registered)
			return;

		WNDCLASS const windowClass
		{
			//.style = CS_HREDRAW | CS_VREDRAW,
			.lpfnWndProc = windowProc,
			.cbWndExtra = sizeof(SplashWindow*),
			/*.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH),*/
			.lpszClassName = className
		};
		RegisterClass(&windowClass);
		registered = true;
	}

	static auto getSelf(HWND hwnd)
	{
		return reinterpret_cast<SplashWindow*>(GetWindowLongPtr(hwnd, 0));
	}

	static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
			case WM_NCCREATE:
			{
				auto createStruct = reinterpret_cast<CREATESTRUCT*>(lparam);
				auto splashWindow = static_cast<SplashWindow*>(createStruct->lpCreateParams);
				SetWindowLongPtr(hwnd, 0, reinterpret_cast<LONG_PTR>(splashWindow));
				break;
			}
			case WM_WINDOWPOSCHANGING:
			{
				if (syncMoveWithWindow)
				{
					auto windowPos = reinterpret_cast<WINDOWPOS*>(lparam);
					if (!(windowPos->flags & SWP_NOMOVE))
					{
						SetWindowPos(
							syncMoveWithWindow,
							nullptr,
							windowPos->x,
							windowPos->y,
							0,
							0,
							SWP_NOSIZE | SWP_NOACTIVATE
						);
					}
				}
				break;
			}
			case WM_ACTIVATE:
			{
				static bool firstTime = true;
				if (firstTime)
				{
					firstTime = false;
					StartupTimer::GetInstance().SetWindowShown();
					StartupTimer::GetInstance().Print(argv);
				}
				break;
			}
			case WM_SIZE:
			{
				auto self = getSelf(hwnd);
				//TODO: handle resize
				break;
			}
			case WM_USER + 1:
			{
				syncMoveWithWindow = reinterpret_cast<HWND>(wparam);
				auto self = getSelf(hwnd);
				self->background.StartToFade(self->opacityAnimation);
				self->logoSprite.StartAnimation(L"Opacity", self->opacityAnimation);
				
				return 0;
			}
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
			default:
				break;
		}
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	auto getInitialWindowSize()
	{
		//TODO：We'd better remember the window size to mimic UWP behavior
		return D2D1::SizeU(800, 600);
	}

	void createWindow()
	{
		auto const [width, height] = getInitialWindowSize();

		RECT rect{ .right = static_cast<LONG>(width), .bottom = static_cast<LONG>(height) };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

		m_hwnd = CreateWindowEx(
			WS_EX_NOREDIRECTIONBITMAP,
			className,
			L"SplashScreenDemo",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			rect.right - rect.left,
			rect.bottom - rect.top,
			nullptr,
			nullptr,
			nullptr,
			this
		);

		m_compositionWrapper.emplace(m_hwnd);
		auto dpi = GetDpiForWindow(m_hwnd);
		LogoCompositionSurface logoSurface{ m_compositionWrapper->GetInterop().get(), width, height };
		
		auto logoSurfaceBrush = m_compositionWrapper->m_compositor.CreateSurfaceBrush(logoSurface.m_drawingSurface);
		logoSprite = m_compositionWrapper->m_compositor.CreateSpriteVisual();
		logoSprite.Size({ static_cast<float>(width), static_cast<float>(height) });
		logoSprite.Brush(logoSurfaceBrush);
		m_compositionWrapper->visuals.InsertAtTop(logoSprite);

		
		//new
		background = Background{
			m_compositionWrapper->m_compositor,
			winrt::Windows::Foundation::Numerics::float2{
				static_cast<float>(width),
				static_cast<float>(height)
			},
			m_compositionWrapper->visuals
		};

		opacityAnimation = DoubleAnimationUsingKeyFrames{
			m_compositionWrapper->m_compositor,
			std::chrono::milliseconds{300},
			ScalarKeyFrame{ .normalizedProgressKey = 1.0f }
		};

		progressBar = ProgressBarComposition{
			m_compositionWrapper->m_compositor,
			{Config::ProgressBarWidth, Config::ProgressBarHeight},
			m_compositionWrapper->visuals
		};

		messageQueue.emplace(m_hwnd);

		//Create d2d

		ShowWindow(m_hwnd, SW_SHOW);
	}
public:
	constexpr static auto AnimationTimerId = 0x2;
	constexpr static auto MessageQueuePollingTimerIntervalMilliseconds = 100;
	constexpr static auto AnimationTimerInterval = 5;

	SplashWindow()
	{
		StartupTimer::GetInstance().SetBeforeMainWindowConstructed();
		registerClassIfNeeded();

		createWindow();
	}
};



int WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
)
{
	THROW_IF_FAILED(CoInitialize(nullptr));
	StartupTimer::GetInstance();
	argv = lpCmdLine;

	SplashWindow window;

	MSG msg;
	BOOL ret;
	while ((ret = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (ret == -1)
		{
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	SetForegroundWindow(syncMoveWithWindow);
}
