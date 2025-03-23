#include "pch.h"
#include "SplashScreenComponentMessageQueue.h"

SplashScreenComponentMessageQueue::SplashScreenComponentMessageQueue(std::wstring_view name) :
	boost::interprocess::message_queue{boost::interprocess::open_only, name.data()},
	m_name{name}
{
	uint64_t hwnd{};
	swscanf(name.data(), L"SplashScreenDemo_%llu", &hwnd);
	m_splashScreenHwnd = reinterpret_cast<HWND>(hwnd);
}

void SplashScreenComponentMessageQueue::SendHwnd(HWND hwnd)
{
	send(&hwnd, sizeof(hwnd), 0);
}

SplashScreenComponentMessageQueue::~SplashScreenComponentMessageQueue()
{
	remove(m_name.data());
}

HWND SplashScreenComponentMessageQueue::splashScreenHwnd() const
{
	return m_splashScreenHwnd;
}
