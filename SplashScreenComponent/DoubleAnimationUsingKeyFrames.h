#pragma once
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Composition.h>
#include <initializer_list>
#include "ScalarKeyFrame.h"

class DoubleAnimationUsingKeyFrames : public winrt::Windows::UI::Composition::ScalarKeyFrameAnimation
{
public:
	DoubleAnimationUsingKeyFrames(
		winrt::Windows::UI::Composition::Compositor const& compositor,
		winrt::Windows::Foundation::TimeSpan duration,
		std::initializer_list<ScalarKeyFrame> const& keyFrames
	);

	DoubleAnimationUsingKeyFrames(std::nullptr_t);
};
