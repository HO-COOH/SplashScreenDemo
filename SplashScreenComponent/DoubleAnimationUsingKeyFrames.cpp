#include "DoubleAnimationUsingKeyFrames.h"

DoubleAnimationUsingKeyFrames::DoubleAnimationUsingKeyFrames(
	winrt::Windows::UI::Composition::Compositor const& compositor, 
	winrt::Windows::Foundation::TimeSpan duration, 
	std::initializer_list<ScalarKeyFrame> const& keyFrames
) : ScalarKeyFrameAnimation{ compositor.CreateScalarKeyFrameAnimation() }
{
	Duration(duration);
	for (auto const keyFrame : keyFrames)
		InsertKeyFrame(keyFrame.normalizedProgressKey, keyFrame.value);
}

DoubleAnimationUsingKeyFrames::DoubleAnimationUsingKeyFrames(std::nullptr_t) : ScalarKeyFrameAnimation{ nullptr }
{
}
