#include "pch.h"
#include <format>
#include <sstream>
#include "StartupTimer.h"

StartupTimer& StartupTimer::GetInstance()
{
    static StartupTimer s_instance;
    return s_instance;
}

void StartupTimer::SetStartTime(std::wstring_view arg)
{
    m_startupTime = arg;
}

void StartupTimer::SetAppLaunch()
{
    m_inAppLaunch = std::chrono::system_clock::now();
}

void StartupTimer::SetBeforeMainWindowConstructed()
{
    m_beforeMainWindowConstructed = std::chrono::system_clock::now();
}

void StartupTimer::SetWindowShown()
{
    m_windowShown = std::chrono::system_clock::now();
}

void StartupTimer::Print()
{
    if (!m_startupTime.empty())
    {
        std::wistringstream ss{ std::wstring{m_startupTime} };
        std::chrono::system_clock::rep count;
        ss >> count;
        m_constructed = std::chrono::system_clock::time_point{std::chrono::system_clock::duration{count} };
    }
    auto str = std::format(
        L"App.OnLaunch: {} ms\n"
        L"MainWindow constructor: {} ms\n"
        L"MainWindow shown: {} ms", 
        std::chrono::duration_cast<std::chrono::milliseconds>(m_inAppLaunch - m_constructed).count(),
        std::chrono::duration_cast<std::chrono::milliseconds>(m_beforeMainWindowConstructed - m_constructed).count(),
        std::chrono::duration_cast<std::chrono::milliseconds>(m_windowShown - m_constructed).count()
    );

    OutputDebugString(str.data());
    MessageBox(NULL, str.data(), L"Startup time: ", 0);
}
