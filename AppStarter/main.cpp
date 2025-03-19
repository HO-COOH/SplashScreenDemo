#include <iostream>
#include <format>
#include <chrono>

void LaunchCpp(int times)
{
	for (int i = 0; i < times; ++i)
	{
		std::cout << "C++ #" << i << '\n';
		std::system(std::format("splashscreendemocpp.exe {}", std::chrono::system_clock::now().time_since_epoch().count()).data());
	}
}

void LaunchCSharp(int times)
{
	for (int i = 0; i < times; ++i)
	{
		std::cout << "C# #" << i << '\n';
		std::system(std::format("splashscreendemocs.exe {}", std::chrono::system_clock::now().time_since_epoch().count() / 10000).data());
	}
}

void LaunchSplashComponent(int times)
{
	for (int i = 0; i < times; ++i)
	{
		std::cout << "SplashComponent #" << i << '\n';
		std::system(std::format("SplashScreenComponent.exe {}", std::chrono::system_clock::now().time_since_epoch().count()).data());
	}
}

int main()
{
	LaunchCpp(5);
	LaunchCSharp(5);
	LaunchSplashComponent(5);
}
