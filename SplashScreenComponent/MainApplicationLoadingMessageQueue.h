#pragma once
#include <boost/interprocess/ipc/message_queue.hpp>
#include <Windows.h>
#include <format>
#include <chrono>

class MainApplicationLoadingMessageQueue : public boost::interprocess::message_queue
{
	static inline auto timeStamp = std::chrono::steady_clock::now().time_since_epoch().count();
	template<typename Char>
	static std::basic_string<Char> generateTimeStampedName(HWND splashScreenHwnd)
	{
		if constexpr (std::is_same_v<Char, char>)
		{
			return std::format("SplashScreenDemo_{}_{}", reinterpret_cast<uint64_t>(splashScreenHwnd), timeStamp);
		}
		else
		{
			return std::format(L"SplashScreenDemo_{}_{}", reinterpret_cast<uint64_t>(splashScreenHwnd), timeStamp);
		}
	}
public:
	MainApplicationLoadingMessageQueue(HWND splashScreenHwnd);
};
