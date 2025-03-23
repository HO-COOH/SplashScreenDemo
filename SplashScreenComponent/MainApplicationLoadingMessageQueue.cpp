#include "MainApplicationLoadingMessageQueue.h"
#include <format>
#include <chrono>
#include <thread>

std::string const& MainApplicationLoadingMessageQueue::generateTimeStampedName(HWND splashScreenHwnd)
{
	 static auto str = std::format("SplashScreenDemo_{}_{}", reinterpret_cast<uint64_t>(splashScreenHwnd), std::chrono::steady_clock::now().time_since_epoch().count());
	 return str;
}

MainApplicationLoadingMessageQueue::MainApplicationLoadingMessageQueue(HWND splashScreenHwnd)
	: boost::interprocess::message_queue{ boost::interprocess::create_only, generateTimeStampedName(splashScreenHwnd).data(), 1, sizeof(void*) }
{
	std::thread{
		[splashScreenHwnd] {std::system(std::format("splashscreendemocpp.exe {}", generateTimeStampedName(splashScreenHwnd)).data()); }
	}.detach();
}
