#include "SplashWindow.h"
#include "StartupTimer.h"
#include "Config.h"
#include "MainApplicationLoadingMessageQueue.h"
#include "SvgSprite.h"
#include "WindowSizeManager.h"
#include <d2d1_2.h>
#include "D2D1Factory.h"
#include "Glyphs.h"
#include <windowsx.h> // for GET_Y_LPARAM() macro
#include "DpiUtils.hpp"

extern LPSTR argv;

HWND SplashWindow::syncMoveWithWindow;

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
	{
		self->background.Size({ static_cast<float>(width), static_cast<float>(height) });
		auto const clientRect = self->ClientRect();
		auto const dpi = self->GetDpi();
		winrt::Windows::Foundation::Numerics::float2 const scaledButtonSize
		{
			ScaleForDpi(Config::CaptionButtonWidth, dpi),
			ScaleForDpi(Config::CaptionButtonHeight, dpi)
		};
		self->minimizeButton.Offset({ width - scaledButtonSize.x * 3, 0, 0 });
		self->maximizeButton.Offset({ width - scaledButtonSize.x * 2, 0, 0 });
		self->closeButton.Offset({ width - scaledButtonSize.x, 0, 0 });
		self->m_compositionWrapper->containerVisual.Size({ static_cast<float>(width), static_cast<float>(height)});
		self->logo.Size({ ScaleForDpi(Config::LogoWidth, dpi), ScaleForDpi(Config::LogoHeight, dpi) });
		self->logo.Offset({ width / 2.0f, height / 2.0f, 0 });
	}
}

void SplashWindow::OnMouseMove(HWND hwnd, WPARAM buttonDown, WORD x, WORD y)
{
	auto self = getSelfFromHwnd(hwnd);
	auto dpi = self->GetDpi();
	if (!self->maximizeButton.HitTest(x, y, dpi)) self->maximizeButton.PlayColorAnimation(self->pointerExitCaptoinButtonColorAnimation);
	if (!self->minimizeButton.HitTest(x, y, dpi)) self->minimizeButton.PlayColorAnimation(self->pointerExitCaptoinButtonColorAnimation);
	if (!self->closeButton.HitTest(x, y, dpi)) self->closeButton.PlayColorAnimation(self->pointerExitCaptoinButtonColorAnimation);
}

LRESULT SplashWindow::OnUserMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_USER + 1)
	{
		syncMoveWithWindow = reinterpret_cast<HWND>(wparam);
		//getSelfFromHwnd(hwnd)->onMainAppLoaded();
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

	self->m_resizeHandleHeight = GetSystemMetricsForDpi(SM_CYSIZE, dpiX) + GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpiX) + GetSystemMetricsForDpi(SM_CYFRAME, dpiX);
	BaseWindow::OnDpiChanged(hwnd, dpiX, dpiY, suggestedPosition);

	auto const clientRect = self->ClientRect();
	self->background.Size(winrt::Windows::Foundation::Numerics::float2{
		static_cast<float>(clientRect.right),
		static_cast<float>(clientRect.bottom)
	});

	self->titleText.SetText(
	self->m_compositionWrapper->m_compositor,
	self->m_compositionWrapper->GetGraphicsDevice(),
	L"SplashScreenDemo",
	ScaleForDpi(Config::CaptionTextFontSize, dpiX)
	);
	self->titleText.Offset(ScaleForDpi(Config::CaptionTextOffset, dpiX));
}

LRESULT SplashWindow::OnNCCalcSize(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	if (!wparam)
	{
		return 0;
	}

	auto params = reinterpret_cast<NCCALCSIZE_PARAMS*>(lparam);

	// Store the original top before the default window proc applies the
	// default frame.
	const auto originalTop = params->rgrc[0].top;

	const auto originalSize = params->rgrc[0];

	// apply the default frame
	const auto ret = DefWindowProc(hwnd, WM_NCCALCSIZE, wparam, lparam);
	if (ret != 0)
	{
		return ret;
	}

	auto newSize = params->rgrc[0];
	// Re-apply the original top from before the size of the default frame was applied.
	newSize.top = originalTop;

	//// WM_NCCALCSIZE is called before WM_SIZE
	//_UpdateMaximizedState();

	//// We don't need this correction when we're fullscreen. We will have the
	//// WS_POPUP size, so we don't have to worry about borders, and the default
	//// frame will be fine.
	//if (_isMaximized && !_fullscreen)
	//{
	//	// When a window is maximized, its size is actually a little bit more
	//	// than the monitor's work area. The window is positioned and sized in
	//	// such a way that the resize handles are outside of the monitor and
	//	// then the window is clipped to the monitor so that the resize handle
	//	// do not appear because you don't need them (because you can't resize
	//	// a window when it's maximized unless you restore it).
	//	newSize.top += _GetResizeHandleHeight();
	//}

	params->rgrc[0] = newSize;
	return 0;
}

LRESULT SplashWindow::OnNCHitTest(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	if (auto const originalNCHitTestResult = DefWindowProc(hwnd, WM_NCHITTEST, wparam, lparam);originalNCHitTestResult != HTCLIENT)
		return originalNCHitTestResult;

	auto self = getSelfFromHwnd(hwnd);

	POINT p{ GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) };
	winrt::check_bool(ScreenToClient(hwnd, &p));
	if (p.y <= self->m_resizeHandleHeight)
	{
		auto const dpi = self->GetDpi();
		if (self->maximizeButton.HitTest(p.x, p.y, dpi))
			return HTMAXBUTTON;
		if (self->minimizeButton.HitTest(p.x, p.y, dpi))
			return HTMINBUTTON;
		if (self->closeButton.HitTest(p.x, p.y, dpi))
			return HTCLOSE;
		return HTCAPTION;
	}
	return HTCLIENT;
}

LRESULT SplashWindow::OnNCMouseMove(HWND hwnd, WPARAM hitTestResult, LPARAM point)
{
	RECT windowRect;
	GetWindowRect(hwnd, &windowRect);
	OnMouseMove(hwnd, NULL, GET_X_LPARAM(point) - windowRect.left, GET_Y_LPARAM(point) - windowRect.top);
	return 0;
}

void SplashWindow::onMainAppLoaded()
{
	//background.StartToFade(opacityAnimation);
	//logo.StartAnimation(L"Opacity",opacityAnimation);
	m_compositionWrapper->containerVisual.StartAnimation(L"Opacity", opacityAnimation);
}

std::wstring SplashWindow::getSyncMoveWindowTitle()
{
	std::wstring text(GetWindowTextLengthW(SplashWindow::syncMoveWithWindow), {});
	GetWindowText(SplashWindow::syncMoveWithWindow, text.data(), text.size() - 1);
	return text;
}


SplashWindow::SplashWindow() : BaseWindow{
	L"SplashWindow", 
	WS_EX_NOREDIRECTIONBITMAP, 
	WS_OVERLAPPEDWINDOW, 
	CW_USEDEFAULT, 
	CW_USEDEFAULT, 
	static_cast<int>(WindowSizeManager::GetWindowSize(GetDpiForSystem()).cx), 
	static_cast<int>(WindowSizeManager::GetWindowSize(GetDpiForSystem()).cy)
}
{
	StartupTimer::GetInstance().SetBeforeMainWindowConstructed();

	auto const [width, height] = WindowSizeManager::GetWindowClientSize();

	RECT rect{ .right = static_cast<LONG>(width), .bottom = static_cast<LONG>(height) };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	m_compositionWrapper.emplace(m_hwnd.get());
	auto dpi = GetDpiForWindow(m_hwnd.get());

	logo = LogoCompositionSurface{ m_compositionWrapper->m_compositor, m_compositionWrapper->GetGraphicsDevice(), ScaleForDpi<UINT>(Config::LogoWidth, dpi), ScaleForDpi<UINT>(Config::LogoHeight, dpi) };
	logo.AnchorPoint({ 0.5f, 0.5f });
	logo.Offset({ width / 2.f, height / 2.f, 0 });
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

	//progressBar = ProgressBarComposition{
	//	m_compositionWrapper->m_compositor,
	//	{Config::ProgressBarWidth, Config::ProgressBarHeight},
	//	m_compositionWrapper->visuals
	//};

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

	minimizeButton.Offset({ width - scaledButtonSize.x * 3, 0, 0 });
	maximizeButton.Offset({ width - scaledButtonSize.x * 2, 0, 0 });
	closeButton.Offset({ width - scaledButtonSize.x, 0, 0 });
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

	titleText = TextBlock{ 
		m_compositionWrapper->m_compositor, 
		m_compositionWrapper->GetGraphicsDevice(), 
		L"SplashScreenDemo", 
		ScaleForDpi(Config::CaptionTextFontSize, dpi) 
	};
	titleText.Offset(ScaleForDpi(Config::CaptionTextOffset, dpi));
	m_compositionWrapper->visuals.InsertAtTop(titleText);

	MainApplicationLoadingMessageQueue{ m_hwnd.get() };

	SetWindowPos(m_hwnd.get(),
		nullptr,
		0, 0, 0, 0,
		SWP_SHOWWINDOW | SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE);

	m_resizeHandleHeight = GetSystemMetricsForDpi(SM_CYSIZE, dpi) + GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi) + GetSystemMetricsForDpi(SM_CYFRAME, dpi);
	auto height2 = GetSystemMetrics(SM_CYSIZE);
}

