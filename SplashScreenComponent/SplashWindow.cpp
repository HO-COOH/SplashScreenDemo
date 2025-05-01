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
	auto self = getSelfFromHwnd(hwnd);
	if (self->background)
		self->background.Size({ static_cast<float>(width), static_cast<float>(height) });
}

void SplashWindow::OnMouseMove(HWND hwnd, WPARAM buttonDown, WORD x, WORD y)
{
	auto self = getSelfFromHwnd(hwnd);
	if (!self->maximizeButton.HitTest(x, y)) self->maximizeButton.PlayColorAnimation(self->pointerExitCaptoinButtonColorAnimation);
	if (!self->minimizeButton.HitTest(x, y)) self->minimizeButton.PlayColorAnimation(self->pointerExitCaptoinButtonColorAnimation);
	if (!self->closeButton.HitTest(x, y)) self->closeButton.PlayColorAnimation(self->pointerExitCaptoinButtonColorAnimation);
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

void SplashWindow::OnDpiChanged(HWND hwnd, WORD dpiX, WORD dpiY, RECT* suggestedPosition)
{
	auto self = getSelfFromHwnd(hwnd);
	winrt::Windows::Foundation::Size const scaledSvgSize
	{
		ScaleForDpi(Config::CaptionButtonGlyphSize, dpiX),
		ScaleForDpi(Config::CaptionButtonGlyphSize, dpiX)
	};
	winrt::Windows::Foundation::Numerics::float2 const scaledButtonSize
	{ 
		ScaleForDpi(Config::CaptionButtonWidth, dpiX),  
		ScaleForDpi(Config::CaptionButtonHeight, dpiX) 
	};

	self->closeButton.SetContent(SvgSprite{ Glyphs::Close, scaledSvgSize, self->m_compositionWrapper->m_compositor, self->m_compositionWrapper->GetGraphicsDevice() });
	self->closeButton.Size(scaledButtonSize);

	self->maximizeButton.SetContent(SvgSprite{ Glyphs::Maximize, scaledSvgSize, self->m_compositionWrapper->m_compositor, self->m_compositionWrapper->GetGraphicsDevice() });
	self->maximizeButton.Size(scaledButtonSize);

	self->minimizeButton.SetContent(SvgSprite{ Glyphs::Minimize, scaledSvgSize, self->m_compositionWrapper->m_compositor, self->m_compositionWrapper->GetGraphicsDevice() });
	self->minimizeButton.Size(scaledButtonSize);

	auto const clientRect = self->ClientRect();
	self->background.Size(winrt::Windows::Foundation::Numerics::float2{ 
		static_cast<float>(clientRect.right), 
		static_cast<float>(clientRect.bottom) 
	});

	BaseWindow::OnDpiChanged(hwnd, dpiX, dpiY, suggestedPosition);
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

	winrt::Windows::Foundation::Numerics::float2 const scaledButtonSize{ ScaleForDpi(Config::CaptionButtonWidth, dpi), ScaleForDpi(Config::CaptionButtonHeight, dpi) };
	{
		ColorAnimationUsingKeyFrames minimizeCaptionButtonColorAnimation
		{
			m_compositionWrapper->m_compositor,
			std::chrono::milliseconds{Config::CaptionButtonColorAnimationDurationMilliseconds},
			{
				ColorKeyFrame{.normalizedProgressKey = 1.f, .color = Config::Dark::CaptionButtonPointerOverColor}
			}
		};

		ColorAnimationUsingKeyFrames closeCaptionButtonColorAnimation
		{
			m_compositionWrapper->m_compositor,
			std::chrono::milliseconds{Config::CaptionButtonColorAnimationDurationMilliseconds},
			{
				ColorKeyFrame{1.f, Config::Dark::CaptionCloseButtonPointerOverColor}
			}
		};

		//draw caption buttons
		winrt::Windows::Foundation::Size const scaledSvgSize{ ScaleForDpi(Config::CaptionButtonGlyphSize, dpi), ScaleForDpi(Config::CaptionButtonGlyphSize, dpi) };
		maximizeButton = CaptionButton{ m_compositionWrapper->m_compositor, SvgSprite{Glyphs::Maximize, scaledSvgSize, m_compositionWrapper->m_compositor, m_compositionWrapper->GetGraphicsDevice()}, scaledButtonSize, minimizeCaptionButtonColorAnimation };
		closeButton = CaptionButton{ m_compositionWrapper->m_compositor, SvgSprite{Glyphs::Close, scaledSvgSize, m_compositionWrapper->m_compositor, m_compositionWrapper->GetGraphicsDevice()}, scaledButtonSize, closeCaptionButtonColorAnimation };
		minimizeButton = CaptionButton{ m_compositionWrapper->m_compositor, SvgSprite{Glyphs::Minimize, scaledSvgSize, m_compositionWrapper->m_compositor, m_compositionWrapper->GetGraphicsDevice()}, scaledButtonSize, minimizeCaptionButtonColorAnimation };
	}


	maximizeButton.Offset({ scaledButtonSize.x, 0, 0 });
	closeButton.Offset({ scaledButtonSize.x * 2, 0, 0 });
	for (auto const& button : {maximizeButton, closeButton, minimizeButton})
		m_compositionWrapper->visuals.InsertAtTop(button);

	pointerExitCaptoinButtonColorAnimation = ColorAnimationUsingKeyFrames
	{
		m_compositionWrapper->m_compositor,
		std::chrono::milliseconds{Config::CaptionButtonColorAnimationDurationMilliseconds},
		{
			ColorKeyFrame{1.f, winrt::Windows::UI::Colors::Transparent() }
		}
	};

	MainApplicationLoadingMessageQueue{ m_hwnd.get() };
}

