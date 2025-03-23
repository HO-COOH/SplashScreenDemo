#include "pch.h"
#include <format>
#include <sstream>
#include "StartupTimer.h"

StartupTimer& StartupTimer::GetInstance()
{
    static StartupTimer s_instance;
    return s_instance;
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
    auto cmd = GetCommandLine();
    int argc{};
    auto argv = CommandLineToArgvW(cmd, &argc);
    if (argc <= 1 || std::wstring_view{argv[1]}.starts_with(L"SplashScreenDemo_"))
    {
        return;
    }

    std::wistringstream ss{ std::wstring{argv[1]} };
    std::chrono::system_clock::rep count;
    ss >> count;
    auto const commandLaunched = std::chrono::system_clock::time_point{ std::chrono::system_clock::duration{count} };


    auto str = std::format(
        L"main(): {} ms\n"
        L"App.OnLaunch: {} ms\n"
        L"MainWindow constructor: {} ms\n"
        L"MainWindow shown: {} ms",
		std::chrono::duration_cast<std::chrono::milliseconds>(m_constructed - commandLaunched).count(),
        std::chrono::duration_cast<std::chrono::milliseconds>(m_inAppLaunch - commandLaunched).count(),
        std::chrono::duration_cast<std::chrono::milliseconds>(m_beforeMainWindowConstructed - commandLaunched).count(),
        std::chrono::duration_cast<std::chrono::milliseconds>(m_windowShown - commandLaunched).count()
    );

    OutputDebugString(str.data());
    MessageBox(NULL, str.data(), L"Startup time: ", 0);
}
