#include "CaptionButton.h"
#include "Config.h"

void CaptionButton::Offset(winrt::Windows::Foundation::Numerics::float3 offset)
{
	m_offset = offset;
	Button::Offset(offset);
}

bool CaptionButton::HitTest(WORD x, WORD y)
{
	if (m_offset.x < x &&
		m_offset.x + Config::CaptionButtonWidth > x &&
		m_offset.y < y &&
		m_offset.y + Config::CaptionButtonHeight > y)
	{
		PlayColorAnimation(m_colorAnimation);
		return true;
	}

	return false;
}
