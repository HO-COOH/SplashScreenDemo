#include "pch.h"
#include <format>
#include "StartupTimer.h"

StartupTimer& StartupTimer::GetInstance()
{
    static StartupTimer s_instance;
    return s_instance;
}

void StartupTimer::Print()
{
    auto str = std::format(L"{} ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_constructed).count());
    OutputDebugString(str.data());
    MessageBox(NULL, str.data(), L"Startup time: ", 0);
}
