#include "pch.h"
#include "SplashScreenComponentMessageQueue.h"

SplashScreenComponentMessageQueue::SplashScreenComponentMessageQueue(std::wstring_view name) :
	m_name{name}
{
	uint64_t hwnd{};
	swscanf(name.data(), L"SplashScreenDemo_%llu", &hwnd);
	m_splashScreenHwnd = reinterpret_cast<HWND>(hwnd);
}

void SplashScreenComponentMessageQueue::SendHwnd(HWND hwnd)
{
	SendMessage(m_splashScreenHwnd, WM_USER + 1, reinterpret_cast<WPARAM>(hwnd), 0);
}

HWND SplashScreenComponentMessageQueue::splashScreenHwnd() const
{
	return m_splashScreenHwnd;
}
