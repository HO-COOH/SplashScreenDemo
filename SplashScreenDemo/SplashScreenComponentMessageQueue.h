#pragma once
#include <boost/interprocess/ipc/message_queue.hpp>

class SplashScreenComponentMessageQueue 
	: public boost::interprocess::message_queue
{
	HWND m_splashScreenHwnd{};
	std::wstring_view m_name;
public:
	SplashScreenComponentMessageQueue(std::wstring_view name);
	~SplashScreenComponentMessageQueue();

	HWND splashScreenHwnd() const;
	void SendHwnd(HWND hwnd);
};

