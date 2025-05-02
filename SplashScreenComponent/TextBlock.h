#pragma once
#include <winrt/Windows.UI.Composition.h>

class TextBlock : public winrt::Windows::UI::Composition::SpriteVisual
{
public:
	TextBlock(winrt::Windows::UI::Composition::Compositor const& compositor, 
		winrt::Windows::UI::Composition::CompositionGraphicsDevice const& graphicsDevice,
		std::wstring_view text, 
		float fontSize);

	TextBlock(std::nullptr_t) : SpriteVisual{ nullptr } {}
};
