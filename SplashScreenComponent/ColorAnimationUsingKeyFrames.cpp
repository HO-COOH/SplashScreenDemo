#include "ColorAnimationUsingKeyFrames.h"

ColorAnimationUsingKeyFrames::ColorAnimationUsingKeyFrames(
	winrt::Windows::UI::Composition::Compositor const& compositor, 
	winrt::Windows::Foundation::TimeSpan duration, 
	std::initializer_list<ColorKeyFrame> const& keyFrames
) : ColorKeyFrameAnimation{ compositor.CreateColorKeyFrameAnimation() }
{
	Duration(duration);
	for (auto const keyFrame : keyFrames)
		InsertKeyFrame(keyFrame.normalizedProgressKey, keyFrame.color);
}
