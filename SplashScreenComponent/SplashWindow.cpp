#include "SplashWindow.h"
#include "StartupTimer.h"
#include "Config.h"
#include "MainApplicationLoadingMessageQueue.h"
#include "SvgSprite.h"
#include "WindowSizeManager.h"
#include <d2d1_2.h>
#include "D2D1Factory.h"
#include "Glyphs.h"

extern LPSTR argv;

HWND SplashWindow::syncMoveWithWindow;

static auto ScaleForDpi(auto value, UINT dpi)
{
	return static_cast<float>(value * dpi / 96);
};


void SplashWindow::OnWindowPosChanging(HWND hwnd, WINDOWPOS* windowPos)
{
	if (syncMoveWithWindow)
	{
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
}

void SplashWindow::OnActivate(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	static bool firstTime = true;
	if (firstTime)
	{
		firstTime = false;
		StartupTimer::GetInstance().SetWindowShown();
		StartupTimer::GetInstance().Print(argv);
	}
}

void SplashWindow::OnSize(HWND hwnd, WPARAM wparam, UINT width, UINT height)
{
}

LRESULT SplashWindow::OnUserMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_USER + 1)
	{
		syncMoveWithWindow = reinterpret_cast<HWND>(wparam);
		getSelfFromHwnd(hwnd)->onMainAppLoaded();
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void SplashWindow::onMainAppLoaded()
{
	background.StartToFade(opacityAnimation);
	logo.StartAnimation(L"Opacity",opacityAnimation);
}

SplashWindow::SplashWindow() : BaseWindow{
	L"SplashWindow", 
	WS_EX_NOREDIRECTIONBITMAP, 
	WS_OVERLAPPEDWINDOW, 
	CW_USEDEFAULT, 
	CW_USEDEFAULT, 
	static_cast<int>(WindowSizeManager::GetWindowSize().width), 
	static_cast<int>(WindowSizeManager::GetWindowSize().height)
}
{
	StartupTimer::GetInstance().SetBeforeMainWindowConstructed();

	auto const [width, height] = WindowSizeManager::GetWindowSize();

	RECT rect{ .right = static_cast<LONG>(width), .bottom = static_cast<LONG>(height) };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	m_compositionWrapper.emplace(m_hwnd.get());
	auto dpi = GetDpiForWindow(m_hwnd.get());

	logo = LogoCompositionSurface{ m_compositionWrapper->m_compositor, m_compositionWrapper->GetGraphicsDevice(), width, height };
	m_compositionWrapper->visuals.InsertAtTop(logo);


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
		std::chrono::milliseconds{Config::WindowBackgroundAnimationDurationMilliseconds},
		ScalarKeyFrame{.normalizedProgressKey = 1.0f }
	};

	progressBar = ProgressBarComposition{
		m_compositionWrapper->m_compositor,
		{Config::ProgressBarWidth, Config::ProgressBarHeight},
		m_compositionWrapper->visuals
	};

	{
		//draw caption buttons
		captionButton[0] = Button{ m_compositionWrapper->m_compositor, SvgSprite{Glyphs::Restore, {Config::CaptionButtonGlyphSize, Config::CaptionButtonGlyphSize}, m_compositionWrapper->m_compositor, m_compositionWrapper->GetGraphicsDevice()} };
		captionButton[1] = Button{ m_compositionWrapper->m_compositor, SvgSprite{Glyphs::Close, {Config::CaptionButtonGlyphSize, Config::CaptionButtonGlyphSize}, m_compositionWrapper->m_compositor, m_compositionWrapper->GetGraphicsDevice()} };
		captionButton[2] = Button{ m_compositionWrapper->m_compositor, SvgSprite{Glyphs::Maximize, {Config::CaptionButtonGlyphSize, Config::CaptionButtonGlyphSize}, m_compositionWrapper->m_compositor, m_compositionWrapper->GetGraphicsDevice()} };
	}

	captionButton[1].Offset({ 50.f, 0, 0 });
	captionButton[2].Offset({ 100.f, 0, 0 });
	for (auto const& button : captionButton)
		m_compositionWrapper->visuals.InsertAtTop(button);

	MainApplicationLoadingMessageQueue{ m_hwnd.get() };
}

