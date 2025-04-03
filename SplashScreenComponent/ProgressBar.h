#pragma once
#include <wil/com.h>
#include "ComponentBase.h"
#undef GetCurrentTime
#include <winrt/Windows.UI.Xaml.Media.Animation.h>
#include <winrt/Windows.UI.Xaml.Media.h>
#include "Config.h"

#pragma region Forward declaration
struct ID2D1Factory;
struct ID2D1RenderTarget;
struct ID2D1RoundedRectangleGeometry;
struct ID2D1SolidColorBrush;
struct ID2D1Layer;
#pragma endregion

class ProgressBar : public ComponentBase
{
	wil::com_ptr<ID2D1RoundedRectangleGeometry> clipGeometry;
	wil::com_ptr<ID2D1SolidColorBrush> progressBarFillBrush;
	wil::com_ptr<ID2D1Layer> layer;
	wil::com_ptr<ID2D1Layer> layer2;
	winrt::Windows::UI::Xaml::Media::Animation::Storyboard storyboard;
	winrt::Windows::UI::Xaml::Media::TranslateTransform m_translation;
	winrt::Windows::UI::Xaml::Media::TranslateTransform m_translation2;
	constexpr static auto indeterminateProgressBarIndicatorWidth = Config::ProgressBarWidth * 0.4;
	constexpr static auto indeterminateProgressBarIndicatorWidth2 = Config::ProgressBarWidth * 0.6;
	//winrt::SplashScreenComponentRT::DependentValue m_translation;
	//float m_translation{};
public:
	ProgressBar(ID2D1RenderTarget* renderTarget);
	void OnPaint(ID2D1RenderTarget* renderTarget);
	void OnSize(UINT width, UINT height, ID2D1Factory* d2d1Factory);

	void OnTick();
};
