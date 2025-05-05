#include "CaptionButton.h"
#include "Config.h"
#include "DpiUtils.hpp"

void CaptionButton::Offset(winrt::Windows::Foundation::Numerics::float3 offset)
{
	m_offset = offset;
	Button::Offset(offset);
}

bool CaptionButton::HitTest(WORD x, WORD y, UINT dpi)
{
	if (m_offset.x < x &&
		m_offset.x + ScaleForDpi(Config::CaptionButtonWidth, dpi) > x &&
		m_offset.y < y &&
		m_offset.y + ScaleForDpi(Config::CaptionButtonHeight, dpi) > y)
	{
		PlayColorAnimation(m_colorAnimation);
		return true;
	}

	return false;
}
