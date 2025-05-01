#pragma once
#include "BaseWindow.hpp"
#include "Button.h"
#include <optional>
#include "CompositionWrapper.h"
#include "ProgressBarComposition.h"
#include "DoubleAnimationUsingKeyFrames.h"
#include "Background.h"
#include "LogoCompositionSurface.h"
#include "CaptionButton.h"
#include "ColorAnimationUsingKeyFrames.h"

//WIP
class SplashWindow : public BaseWindow<SplashWindow, L"SplashWindowClass">
{
	std::optional<CompositionWrapper> m_compositionWrapper;

	Background background{ nullptr };
	DoubleAnimationUsingKeyFrames opacityAnimation{ nullptr };
	ProgressBarComposition progressBar{ nullptr };
	LogoCompositionSurface logo{ nullptr };
	//Button captionButton[3]{ nullptr, nullptr, nullptr }; 
	CaptionButton minimizeButton{ nullptr };
	CaptionButton maximizeButton{ nullptr };
	CaptionButton closeButton{ nullptr };
	ColorAnimationUsingKeyFrames pointerExitCaptoinButtonColorAnimation{ nullptr };

	//play opacity fade animation
	void onMainAppLoaded();
public:
	constexpr static auto AnimationTimerId = 0x2;
	constexpr static auto MessageQueuePollingTimerIntervalMilliseconds = 100;
	constexpr static auto AnimationTimerInterval = 5;

	SplashWindow();

	static void OnWindowPosChanging(HWND hwnd, WINDOWPOS* windowPos);
	static void OnActivate(HWND hwnd, WPARAM wparam, LPARAM lparam);
	static void OnSize(HWND hwnd, WPARAM wparam, UINT width, UINT height);
	static void OnMouseMove(HWND hwnd, WPARAM buttonDown, WORD x, WORD y);
	static LRESULT OnUserMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static void OnDpiChanged(HWND hwnd, WORD dpiX, WORD dpiY, RECT* suggestedPosition);

	static HWND syncMoveWithWindow;
};
