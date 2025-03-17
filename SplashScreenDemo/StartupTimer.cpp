#include "pch.h"
#include <format>
#include <sstream>
#include "StartupTimer.h"

StartupTimer& StartupTimer::GetInstance()
{
    static StartupTimer s_instance;
    return s_instance;
}

void StartupTimer::ParseStartTime(std::wstring_view arg)
{
    m_arg = arg;
}

void StartupTimer::Print()
{
    std::chrono::nanoseconds diff;
    if (!m_arg.empty())
    {
        auto now = std::chrono::system_clock::now();
        std::wistringstream ss{ std::wstring{m_arg} };
        std::chrono::system_clock::rep count;
        ss >> count;
        diff = now - std::chrono::system_clock::time_point{std::chrono::system_clock::duration{count} };
    }
    else
    {
        diff = std::chrono::system_clock::now() - m_constructed;
    }
    auto str = std::format(L"{} ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(diff).count());
    OutputDebugString(str.data());
    MessageBox(NULL, str.data(), L"Startup time: ", 0);
}
