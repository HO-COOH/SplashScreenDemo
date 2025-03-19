#include "StartupTimer.h"
#include <format>
#include <sstream>
#include <Windows.h>

StartupTimer& StartupTimer::GetInstance()
{
    static StartupTimer s_instance;
    return s_instance;
}

void StartupTimer::SetBeforeMainWindowConstructed()
{
    m_beforeMainWindowConstructed = std::chrono::system_clock::now();
}

void StartupTimer::SetWindowShown()
{
    m_windowShown = std::chrono::system_clock::now();
}

void StartupTimer::Print(char const* argv)
{
    std::istringstream ss{ std::string{argv} };
    std::chrono::system_clock::rep count;
    ss >> count;
    auto const commandLaunched = std::chrono::system_clock::time_point{ std::chrono::system_clock::duration{count} };


    auto str = std::format(
        "main(): {} ms\n"
        "MainWindow constructor: {} ms\n"
        "MainWindow shown: {} ms",
        std::chrono::duration_cast<std::chrono::milliseconds>(m_constructed - commandLaunched).count(),
        std::chrono::duration_cast<std::chrono::milliseconds>(m_beforeMainWindowConstructed - commandLaunched).count(),
        std::chrono::duration_cast<std::chrono::milliseconds>(m_windowShown - commandLaunched).count()
    );

    OutputDebugStringA(str.data());
    MessageBoxA(NULL, str.data(), "Startup time: ", 0);
}
