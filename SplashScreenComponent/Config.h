#pragma once
#include <d2d1helper.h>

#if __has_include("winrt/Windows.UI.h")
#include <winrt/Windows.UI.h>
#include <winrt/Windows.Foundation.Numerics.h>
#endif
#include <optional>

namespace Config
{
#if __has_include("winrt/Windows.UI.h")
	constexpr static winrt::Windows::UI::Color ProgressBarFillColor{ .A = 0xFF, .B = 0xFF };
	namespace Light
	{
		constexpr static winrt::Windows::UI::Color CaptionButtonDefaultColor{};
		constexpr static winrt::Windows::UI::Color CaptionButtonPointerOverColor{.A = 0x09, .R = 0xFF, .G = 0xFF, .B = 0xFF}; //SubtleFillColorSecondary
		constexpr static winrt::Windows::UI::Color CaptionButtonPointerPressedColor{.A = 0x0A, .R = 0xFF, .G = 0xFF, .B = 0xFF};//SubtleFillColorTertiary

		constexpr static winrt::Windows::UI::Color CaptionCloseButtonPointerOverColor{ .A = 0xFF, .R = 0xC4, .G = 0x2B, .B = 0x1C };
		constexpr static winrt::Windows::UI::Color CaptionCloseButtonPointerPressedColor{ .A = static_cast<uint8_t>(0xFF * 0.9), .R = CaptionCloseButtonPointerOverColor.R, .G = CaptionCloseButtonPointerOverColor.G, .B = CaptionCloseButtonPointerOverColor.B };
	}
	namespace Dark
	{
		constexpr static winrt::Windows::UI::Color CaptionButtonDefaultColor{};
		constexpr static winrt::Windows::UI::Color CaptionButtonPointerOverColor{ .A = 0x0F, .R = 0xFF, .G = 0xFF, .B = 0xFF };
		constexpr static winrt::Windows::UI::Color CaptionButtonPointerPressedColor{ .A = 0x06 };

		constexpr static winrt::Windows::UI::Color CaptionCloseButtonPointerOverColor{ .A = 0xFF, .R = 0xC4, .G = 0x2B, .B = 0x1C };
		constexpr static winrt::Windows::UI::Color CaptionCloseButtonPointerPressedColor{ .A = static_cast<uint8_t>(0xFF * 0.9), .R = CaptionCloseButtonPointerOverColor.R, .G = CaptionCloseButtonPointerOverColor.G, .B = CaptionCloseButtonPointerOverColor.B };
	}
#else
	static inline auto ProgressBarFillColor = D2D1::ColorF(0x0000FF);
#endif
	constexpr static auto ProgressBarWidth = 300.f;
	constexpr static auto ProgressBarHeight = 5.f;
	constexpr static auto ProgressBarRadius = 4.f;
	constexpr static auto ProgressBarMarginBottom = 100.f;
	constexpr static auto LogoWidth = 1240;
	constexpr static auto LogoHeight = 600;
	//constexpr static auto LogoPath = LR"(D:\SplashScreenDemo\SplashScreenDemo (Package)\Images\StoreLogo.png)";
	constexpr  static auto LogoFile = L"SplashScreen.scale-200.png";
	constexpr static unsigned int BackgroundColor = 0x0078B4;
	constexpr static auto ExtendsContentIntoTitleBar = false;

	constexpr static auto CaptionButtonGlyphSize = 10;
	constexpr static auto CaptionButtonWidth = 46;
	constexpr static auto CaptionButtonHeight = 32;
	
	constexpr static auto WindowBackgroundAnimationDurationMilliseconds = 200;
	constexpr static auto CaptionButtonColorAnimationDurationMilliseconds = 200;

	constexpr static auto CaptionTextFontSize = 13.f;
	constexpr static winrt::Windows::Foundation::Numerics::float3 CaptionTextOffset{ 10, 7, 0 };
}