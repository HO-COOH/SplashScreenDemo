#pragma once
#include <winrt/Windows.UI.Composition.h>

template<typename ValueType>
struct KeyFrameBase
{
	float normalizedProgressKey;
	ValueType value;
};

template<typename ValueType>
struct EasingKeyFrameBase : public KeyFrameBase<ValueType>
{
	winrt::Windows::UI::Composition::CompositionEasingFunction easingFunction;
};