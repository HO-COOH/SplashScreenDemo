#include "ProgressBar.h"
#include <d2d1.h>

#include <wil/result.h>
#include <wil/com.h>
#include "SplineDoubleKeyFrame.h"

#include <winrt/Windows.UI.Xaml.Media.Animation.h>
#include <winrt/Windows.Foundation.Collections.h>

ProgressBar::ProgressBar(ID2D1RenderTarget* renderTarget)
{
	THROW_IF_FAILED(renderTarget->CreateSolidColorBrush(Config::ProgressBarFillColor, progressBarFillBrush.put()));

	//From src/controls/dev/ProgressBar/ProgressBar.cpp UpdateWidthBasedTemplateSettings()

	auto constexpr containerAnimationStartPosition = indeterminateProgressBarIndicatorWidth * -1.0;
	auto constexpr containerAnimationEndPosition = indeterminateProgressBarIndicatorWidth * 3.0;


	auto constexpr container2AnimationStartPosition = indeterminateProgressBarIndicatorWidth2 * -1.5;
	auto constexpr container2AnimationEndPosition = indeterminateProgressBarIndicatorWidth2 * 1.66;


	winrt::Windows::UI::Xaml::Media::Animation::DiscreteDoubleKeyFrame keyFrame1;
	keyFrame1.Value(containerAnimationStartPosition);
	winrt::Windows::UI::Xaml::Media::Animation::SplineDoubleKeyFrame keyFrame2;
	keyFrame2.KeyTime(winrt::Windows::UI::Xaml::Media::Animation::KeyTimeHelper::FromTimeSpan(std::chrono::milliseconds{ 1500 }));
	keyFrame2.Value(containerAnimationEndPosition);
	winrt::Windows::UI::Xaml::Media::Animation::KeySpline spline;
	spline.ControlPoint1({ 0.4, 0.0 });
	spline.ControlPoint2({ 0.6, 1.0 });
	keyFrame2.KeySpline(spline);
	winrt::Windows::UI::Xaml::Media::Animation::DiscreteDoubleKeyFrame keyFrame3;
	keyFrame3.KeyTime(winrt::Windows::UI::Xaml::Media::Animation::KeyTimeHelper::FromTimeSpan(std::chrono::seconds{2}));
	keyFrame3.Value(containerAnimationEndPosition);

	winrt::Windows::UI::Xaml::Media::Animation::DoubleAnimationUsingKeyFrames animation;
	animation.KeyFrames().ReplaceAll({ keyFrame1, keyFrame2, keyFrame3 });
	animation.EnableDependentAnimation(true);
	m_translation.X(0);
	winrt::Windows::UI::Xaml::Media::Animation::Storyboard::SetTargetProperty(animation, L"X");
	winrt::Windows::UI::Xaml::Media::Animation::Storyboard::SetTarget(animation, m_translation);


	winrt::Windows::UI::Xaml::Media::Animation::DiscreteDoubleKeyFrame keyFrame4;
	keyFrame4.Value(container2AnimationStartPosition);
	winrt::Windows::UI::Xaml::Media::Animation::DiscreteDoubleKeyFrame keyFrame5;
	keyFrame5.Value(container2AnimationStartPosition);
	keyFrame5.KeyTime(winrt::Windows::UI::Xaml::Media::Animation::KeyTimeHelper::FromTimeSpan(std::chrono::milliseconds{ 750 }));
	winrt::Windows::UI::Xaml::Media::Animation::SplineDoubleKeyFrame keyFrame6;
	keyFrame6.Value(container2AnimationEndPosition);
	keyFrame6.KeyTime(winrt::Windows::UI::Xaml::Media::Animation::KeyTimeHelper::FromTimeSpan(std::chrono::seconds{ 2 }));
	winrt::Windows::UI::Xaml::Media::Animation::KeySpline spline2;
	spline2.ControlPoint1({ 0.4, 0.0 });
	spline2.ControlPoint2({ 0.6, 1.0 });
	keyFrame6.KeySpline(spline2);
	
	winrt::Windows::UI::Xaml::Media::Animation::DoubleAnimationUsingKeyFrames animation2;
	animation2.KeyFrames().ReplaceAll({ keyFrame4, keyFrame5, keyFrame6 });
	animation2.EnableDependentAnimation(true);
	winrt::Windows::UI::Xaml::Media::Animation::Storyboard::SetTargetProperty(animation2, L"X");
	winrt::Windows::UI::Xaml::Media::Animation::Storyboard::SetTarget(animation2, m_translation2);


	storyboard.RepeatBehavior(winrt::Windows::UI::Xaml::Media::Animation::RepeatBehaviorHelper::Forever());
	storyboard.Children().ReplaceAll({ animation, animation2 });
	storyboard.Begin();

	THROW_IF_FAILED(renderTarget->CreateLayer(layer.put()));
	THROW_IF_FAILED(renderTarget->CreateLayer(layer2.put()));
}

void ProgressBar::OnPaint(ID2D1RenderTarget* renderTarget)
{
	renderTarget->PushLayer(
		D2D1::LayerParameters(D2D1::InfiniteRect(), clipGeometry.get()),
		layer.get()
	);
	auto x = m_translation.X();
	renderTarget->SetTransform(D2D1::Matrix3x2F::Translation(x, 0.f));

	//draw progress bar
	auto left = (m_width - Config::ProgressBarWidth) / 2.f;
	auto top = m_height - Config::ProgressBarMarginBottom;
	renderTarget->FillRoundedRectangle(
		D2D1::RoundedRect(D2D1::RectF(left, top, left + indeterminateProgressBarIndicatorWidth, top + Config::ProgressBarHeight),
			Config::ProgressBarRadius,
			Config::ProgressBarRadius
		),
		progressBarFillBrush.get()
	);

	renderTarget->PopLayer();

	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	renderTarget->PushLayer(
		D2D1::LayerParameters(D2D1::InfiniteRect(), clipGeometry.get()),
		layer2.get()
	);
	auto x2 = m_translation2.X();
	renderTarget->SetTransform(D2D1::Matrix3x2F::Translation(x2, 0.f));
	renderTarget->FillRoundedRectangle(
		D2D1::RoundedRect(D2D1::RectF(left, top, left + indeterminateProgressBarIndicatorWidth2, top + Config::ProgressBarHeight),
		Config::ProgressBarRadius,
		Config::ProgressBarRadius
		),
		progressBarFillBrush.get()
	);
	renderTarget->PopLayer();
}

void ProgressBar::OnSize(UINT width, UINT height, ID2D1Factory* d2d1Factory)
{
	ComponentBase::OnSize(width, height);

	auto left = (width - Config::ProgressBarWidth) / 2.f;
	auto top = height - Config::ProgressBarMarginBottom;

	THROW_IF_FAILED(d2d1Factory->CreateRoundedRectangleGeometry(
		D2D1::RoundedRect(D2D1::RectF(left, top, left + Config::ProgressBarWidth, top + Config::ProgressBarHeight),
			Config::ProgressBarRadius,
			Config::ProgressBarRadius
		),
		clipGeometry.put()
	));
}

void ProgressBar::OnTick()
{
	//m_translation = m_translation >= Config::ProgressBarWidth ? 0.0f : m_translation + 1.0f;
}

