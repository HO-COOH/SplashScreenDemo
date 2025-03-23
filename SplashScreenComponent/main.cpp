#include <Windows.h>
#include "StartupTimer.h"
#include <optional>
#include "MainApplicationLoadingMessageQueue.h"
#include "D2D1Factory.h"
#include <d2d1.h>
#include <wil/com.h>
#include <wincodec.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include "WICImagingFactory.h"

static LPSTR argv{};
static HWND syncMoveWithWindow{};
static std::optional<MainApplicationLoadingMessageQueue> messageQueue;
static D2D1Factory d2d1Factory;
static std::optional<WICImagingFactory> wicImagingFactory;

//WIP
class SplashWindow
{
	HWND m_hwnd;
	wil::com_ptr<ID2D1HwndRenderTarget> m_renderTarget;
	wil::com_ptr<ID2D1Bitmap> bitmap;

	constexpr static auto className = L"SplashWindowClass";
	static void registerClassIfNeeded()
	{
		static bool registered = false;
		
		if (registered)
			return;

		wicImagingFactory.emplace();

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
			case WM_TIMER:
			{
				if (wparam == SplashWindow::MessageQueuePollingTimerId)
				{
					HWND mainAppHwnd;
					size_t receivedSize{};
					unsigned priority{};
					if (messageQueue->try_receive(&mainAppHwnd, sizeof(mainAppHwnd), receivedSize, priority))
					{
						KillTimer(hwnd, SplashWindow::MessageQueuePollingTimerId);
						syncMoveWithWindow = mainAppHwnd;

						
					}
				}
				return 0;
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
			//case WM_MOVE:
			//{
			//	if (syncMoveWithWindow)
			//	{
			//		auto xPos = (int)(short)LOWORD(lparam);   // horizontal position of client area
			//		auto yPos = (int)(short)HIWORD(lparam);   // vertical position of client area
			//		SetWindowPos(
			//			syncMoveWithWindow,
			//			nullptr,
			//			xPos,
			//			yPos,
			//			0,
			//			0,
			//			SWP_NOSIZE | SWP_NOACTIVATE
			//		);
			//	}
			//	break;
			//}
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
			case WM_PAINT:
			{
				auto self = getSelf(hwnd);
				self->m_renderTarget->BeginDraw();
				self->m_renderTarget->Clear(D2D1::ColorF(0x212121, 0.5f));
				self->m_renderTarget->DrawBitmap(self->bitmap.get());
				THROW_IF_FAILED(self->m_renderTarget->EndDraw());
				ValidateRect(hwnd, nullptr);
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

	auto getLogoFilePath()
	{
		return LR"(C:\Users\Peter\Desktop\SplashScreenDemo\SplashScreenDemo (Package)\Images\Square150x150Logo.scale-200.png)";
	}

	void createWindow()
	{
		auto const [width, height] = getInitialWindowSize();
		m_hwnd = CreateWindowEx(
			WS_EX_COMPOSITED,
			className,
			L"SplashWindow",
			WS_OVERLAPPEDWINDOW,
			0,
			0,
			width,
			height,
			nullptr,
			nullptr,
			nullptr,
			this
		);

		MARGINS m{ -1 };
		DwmExtendFrameIntoClientArea(m_hwnd, &m);

		ShowWindow(m_hwnd, SW_SHOW);
		messageQueue.emplace(m_hwnd);
		THROW_IF_FAILED(d2d1Factory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(
				D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_DEFAULT, 
				D2D1::PixelFormat(DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_PREMULTIPLIED)),
			D2D1::HwndRenderTargetProperties(m_hwnd, D2D1::SizeU(width, height)),
			m_renderTarget.put()
		));

		{
			wil::com_ptr<IWICBitmapDecoder> decoder;
			wil::com_ptr<IWICBitmapFrameDecode> frame;
			THROW_IF_FAILED((*wicImagingFactory)->CreateDecoderFromFilename(
				getLogoFilePath(),
				nullptr,
				GENERIC_READ,
				WICDecodeOptions::WICDecodeMetadataCacheOnDemand,
				decoder.put()));
			THROW_IF_FAILED(decoder->GetFrame(0, frame.put()));
			//convert format to 32bppPBGRA
			wil::com_ptr<IWICFormatConverter> converter;
			THROW_IF_FAILED((*wicImagingFactory)->CreateFormatConverter(converter.put()));
			THROW_IF_FAILED(converter->Initialize(
				frame.get(),
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherType::WICBitmapDitherTypeNone,
				nullptr,
				0.f,
				WICBitmapPaletteType::WICBitmapPaletteTypeCustom));

			THROW_IF_FAILED(m_renderTarget->CreateBitmapFromWicBitmap(
				converter.get(),
				bitmap.put()
			));
		}
		SetTimer(m_hwnd, MessageQueuePollingTimerId, MessageQueuePollingTimerIntervalMilliseconds, nullptr);
	}
public:
	constexpr static auto MessageQueuePollingTimerId = 0x1;
	constexpr static auto MessageQueuePollingTimerIntervalMilliseconds = 100;

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
}
