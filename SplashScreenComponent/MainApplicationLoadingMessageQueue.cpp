#include "MainApplicationLoadingMessageQueue.h"

MainApplicationLoadingMessageQueue::MainApplicationLoadingMessageQueue(HWND splashScreenHwnd)
{
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
