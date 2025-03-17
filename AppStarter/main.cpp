#include <iostream>
#include <format>
#include <chrono>

int main()
{
	std::system(std::format("splashscreendemocpp.exe {}", std::chrono::system_clock::now().time_since_epoch().count()).data());
    std::system(std::format("splashscreendemocs.exe {}", std::chrono::system_clock::now().time_since_epoch().count() / 10000).data());
}
