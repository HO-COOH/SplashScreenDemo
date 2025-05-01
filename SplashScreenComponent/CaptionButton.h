#pragma once
#include "Button.h"
#include "ColorAnimationUsingKeyFrames.h"
#include "SvgSprite.h"
#include <Windows.h>

class CaptionButton : public Button
{
	ColorAnimationUsingKeyFrames m_colorAnimation;
	winrt::Windows::Foundation::Numerics::float3 m_offset{};
public:
	CaptionButton(std::nullptr_t) : Button{ nullptr }, m_colorAnimation{ nullptr } {}

	CaptionButton(
		winrt::Windows::UI::Composition::Compositor const& compositor, 
		SvgSprite&& svg, 
		winrt::Windows::Foundation::Numerics::float2 size,
		ColorAnimationUsingKeyFrames const& colorAnimation)
		: Button{ compositor, svg, size }, m_colorAnimation{ colorAnimation }
	{
	}

	void Offset(winrt::Windows::Foundation::Numerics::float3 offset);

	bool HitTest(WORD x, WORD y);
};
