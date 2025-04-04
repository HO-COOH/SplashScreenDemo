#pragma once
class BackgroundTransparency
{
	float m_value = 1.0f;
	bool m_isFading = false;
public:
	void StartToFade()
	{
		m_isFading = true;
	}

	operator float()
	{
		if (m_isFading)
		{
			m_value -= 0.05f;
			if (m_value < 0)
			{
				m_value = 0;
				m_isFading = false;
			}
		}
		return m_value;
	}
};

