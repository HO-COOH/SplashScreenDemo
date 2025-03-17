#pragma once
#include <chrono>
#include <string_view>
class StartupTimer
{
	std::chrono::system_clock::time_point m_constructed = std::chrono::system_clock::now();
	std::wstring_view m_arg;
public:
	static StartupTimer& GetInstance();

	void ParseStartTime(std::wstring_view arg);

	void Print();
};

