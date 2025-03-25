#include "MainApplicationLoadingMessageQueue.h"

#include <thread>



MainApplicationLoadingMessageQueue::MainApplicationLoadingMessageQueue(HWND splashScreenHwnd)
	: boost::interprocess::message_queue{ boost::interprocess::create_only, generateTimeStampedName<char>(splashScreenHwnd).data(), 1, sizeof(void*) }
{
	//std::thread{
	//	[splashScreenHwnd] {std::system(std::format("splashscreendemocpp.exe {}", generateTimeStampedName(splashScreenHwnd)).data()); }
	//}.detach();

	//auto hr = ShellExecute(
	//	NULL,
	//	L"open",
	//	L"splashscreendemocpp.exe",
	//	generateTimeStampedName<wchar_t>(splashScreenHwnd).data(),
	//	nullptr,
	//	SW_SHOW
	//);

	STARTUPINFO si{};
	PROCESS_INFORMATION pi{};
	CreateProcess(
		nullptr,
		std::format(L"splashscreendemocpp.exe {}", generateTimeStampedName<wchar_t>(splashScreenHwnd)).data(),
		nullptr,
		nullptr,
		false,
		0,
		nullptr,
		nullptr,
		&si,
		&pi
	);
}
