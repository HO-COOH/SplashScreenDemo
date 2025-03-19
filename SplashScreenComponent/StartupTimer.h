#pragma once
#include <chrono>
#include <string_view>

class StartupTimer
{
	std::chrono::system_clock::time_point m_constructed = std::chrono::system_clock::now();
	//std::chrono::system_clock::time_point m_inAppLaunch{};
	std::chrono::system_clock::time_point m_beforeMainWindowConstructed{};
	std::chrono::system_clock::time_point m_windowShown{};
public:
	static StartupTimer& GetInstance();

	//void SetAppLaunch();
	void SetBeforeMainWindowConstructed();
	void SetWindowShown();

	void Print(char const* cmd);
};


