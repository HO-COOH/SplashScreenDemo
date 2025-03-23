#pragma once
#include <boost/interprocess/ipc/message_queue.hpp>
#include <Windows.h>

class MainApplicationLoadingMessageQueue : public boost::interprocess::message_queue
{
	static std::string const& generateTimeStampedName(HWND splashScreenHwnd);
public:
	MainApplicationLoadingMessageQueue(HWND splashScreenHwnd);
};
