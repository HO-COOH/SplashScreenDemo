#pragma once
#include <chrono>
#include <string_view>
class StartupTimer
{
	std::chrono::system_clock::time_point m_constructed = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point m_inAppLaunch{};
	std::chrono::system_clock::time_point m_beforeMainWindowConstructed{};
	std::chrono::system_clock::time_point m_windowShown{};
	std::wstring_view m_startupTime;
public:
	static StartupTimer& GetInstance();

	void SetStartTime(std::wstring_view arg);
	void SetAppLaunch();
	void SetBeforeMainWindowConstructed();
	void SetWindowShown();

	void Print();
};

