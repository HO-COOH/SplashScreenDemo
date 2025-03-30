#pragma once
#include <Windows.h>

class ComponentBase
{
protected:
	UINT m_width{};
	UINT m_height{};
public:
	void OnSize(UINT width, UINT height);
};
