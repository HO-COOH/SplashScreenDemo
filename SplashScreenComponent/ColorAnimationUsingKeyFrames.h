#pragma once
#include "ColorKeyFrame.h"
#include <winrt/Windows.Foundation.h>
#include <initializer_list>
#include <winrt/Windows.UI.Composition.h>

class ColorAnimationUsingKeyFrames : public winrt::Windows::UI::Composition::ColorKeyFrameAnimation
{
public:
	ColorAnimationUsingKeyFrames(
		winrt::Windows::UI::Composition::Compositor const& compositor,
		winrt::Windows::Foundation::TimeSpan duration,
		std::initializer_list<ColorKeyFrame> const& keyFrames
	);

	ColorAnimationUsingKeyFrames(std::nullptr_t) : ColorKeyFrameAnimation{ nullptr } {}
};
