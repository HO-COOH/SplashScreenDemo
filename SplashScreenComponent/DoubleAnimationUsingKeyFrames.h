#pragma once
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Composition.h>
#include "ScalarKeyFrame.h"
#include "KeyFrameAnimationHelper.h"

class DoubleAnimationUsingKeyFrames : 
	public winrt::Windows::UI::Composition::ScalarKeyFrameAnimation, 
	public KeyFrameAnimationHelper<DoubleAnimationUsingKeyFrames, ScalarKeyFrame, EasingScalarKeyFrame>
{
public:
	DoubleAnimationUsingKeyFrames(
		winrt::Windows::UI::Composition::Compositor const& compositor,
		winrt::Windows::Foundation::TimeSpan duration,
		auto&&... keyFrames
	) : ScalarKeyFrameAnimation{ compositor.CreateScalarKeyFrameAnimation() },
		KeyFrameAnimationHelper{ keyFrames... }
	{
		Duration(duration);
	}

	DoubleAnimationUsingKeyFrames(std::nullptr_t) : ScalarKeyFrameAnimation{ nullptr } {}
};
