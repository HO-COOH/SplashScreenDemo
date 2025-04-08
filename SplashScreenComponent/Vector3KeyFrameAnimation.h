#pragma once
#include <winrt/Windows.UI.Composition.h>
#include "Vector3KeyFrame.h"
#include "KeyFrameAnimationHelper.h"

class Vector3KeyFrameAnimation :
    public winrt::Windows::UI::Composition::Vector3KeyFrameAnimation,
    public KeyFrameAnimationHelper<Vector3KeyFrameAnimation, Vector3KeyFrame, EasingVector3KeyFrame>
{
public:
    Vector3KeyFrameAnimation(
		winrt::Windows::UI::Composition::Compositor const& compositor,
		winrt::Windows::Foundation::TimeSpan duration,
		auto&&... keyFrames
	) : winrt::Windows::UI::Composition::Vector3KeyFrameAnimation{ compositor.CreateVector3KeyFrameAnimation() },
		KeyFrameAnimationHelper{ keyFrames... }
	{
		Duration(duration);
	}

    Vector3KeyFrameAnimation(std::nullptr_t) : winrt::Windows::UI::Composition::Vector3KeyFrameAnimation{ nullptr } {}
};