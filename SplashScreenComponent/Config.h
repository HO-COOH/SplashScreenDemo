#pragma once
#include <d2d1helper.h>

namespace Config
{
	static inline auto ProgressBarFillColor = D2D1::ColorF(0x0000FF);
	constexpr static auto ProgressBarWidth = 300.f;
	constexpr static auto ProgressBarHeight = 5.f;
	constexpr static auto ProgressBarRadius = 4.f;
	constexpr static auto ProgressBarMarginBottom = 100.f;
	constexpr static auto LogoWidth = 300;
	constexpr static auto LogoHeight = 300;
	constexpr static auto LogoPath = LR"(C:\Users\Peter\Desktop\SplashScreenDemo\SplashScreenDemo (Package)\Images\Square150x150Logo.scale-200.png)";
	constexpr static unsigned int BackgroundColor = 0xFFB900;
	constexpr static auto ExtendsContentIntoTitleBar = false;
}