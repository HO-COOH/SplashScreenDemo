#pragma once
#include <winrt/Windows.UI.Composition.h>

namespace winrt::Windows::Foundation
{
	struct Size;
}

class SvgSprite : public winrt::Windows::UI::Composition::SpriteVisual
{
public:
	SvgSprite(
		char const* svg,
		winrt::Windows::Foundation::Size const& size,
		winrt::Windows::UI::Composition::Compositor const& compositor,
		winrt::Windows::UI::Composition::CompositionGraphicsDevice const& graphicsDevice);

	SvgSprite(std::nullptr_t) : SpriteVisual{ nullptr } {}
};
