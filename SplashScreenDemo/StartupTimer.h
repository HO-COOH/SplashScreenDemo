#pragma once
#include <chrono>
class StartupTimer
{
	std::chrono::steady_clock::time_point m_constructed = std::chrono::steady_clock::now();
public:
	static StartupTimer& GetInstance();

	void Print();
};

