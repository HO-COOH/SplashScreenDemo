#include "ProgressBarComposition.h"
#include "Config.h"
#include "Vector3KeyFrameAnimation.h"
#include <winrt/Windows.Foundation.Collections.h>

ProgressBarComposition::ProgressBarComposition(
	winrt::Windows::UI::Composition::Compositor const& compositor, 
	winrt::Windows::Foundation::Numerics::float2 const& size, 
	winrt::Windows::UI::Composition::VisualCollection const& visuals) : ShapeVisual{ compositor.CreateShapeVisual() }
{
	auto geometry = compositor.CreateRoundedRectangleGeometry();
	geometry.Size(size);
	geometry.CornerRadius({Config::ProgressBarRadius, Config::ProgressBarRadius});

	//draw the rounded rectangle with composition
	auto roundedRectangle = compositor.CreateSpriteShape(geometry);
	roundedRectangle.FillBrush(compositor.CreateColorBrush(Config::ProgressBarFillColor));
	Shapes().Append(roundedRectangle);
	Size(size);

	visuals.InsertAtTop(*this);


	//animaition
	constexpr float indeterminateProgressBarIndicatorWidth = Config::ProgressBarWidth * 0.4f;
	constexpr float ContainerAnimationStartPosition = indeterminateProgressBarIndicatorWidth * -1.0f;
	constexpr float ContainerAnimationEndPosition = indeterminateProgressBarIndicatorWidth * 3.0f;
	Vector3KeyFrameAnimation animation
	{
		compositor,
		std::chrono::seconds{2},
		Vector3KeyFrame{.normalizedProgressKey = 0.f, .value = {ContainerAnimationStartPosition, 0,0}},
		EasingVector3KeyFrame{
			0.75f,
			{ContainerAnimationEndPosition, 0, 0},
			compositor.CreateCubicBezierEasingFunction({0.4f, 0.0f}, {0.6f, 1.0f})
		},
		Vector3KeyFrame{.normalizedProgressKey = 1.0f, .value = {ContainerAnimationEndPosition, 0, 0}}
	};
	animation.IterationBehavior(winrt::Windows::UI::Composition::AnimationIterationBehavior::Forever);
	StartAnimation(L"Offset", animation);
}
