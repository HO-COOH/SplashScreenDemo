#pragma once
#include <winrt/Windows.UI.h>

struct ColorKeyFrame
{
	float normalizedProgressKey;
	winrt::Windows::UI::Color color;
};
