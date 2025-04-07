#pragma once
#include <Windows.h>
#include <string_view>

class SplashScreenComponentMessageQueue 
{
	HWND m_splashScreenHwnd{};
	std::wstring_view m_name;
public:
	SplashScreenComponentMessageQueue(std::wstring_view name);

	HWND splashScreenHwnd() const;
	void SendHwnd(HWND hwnd);
};

