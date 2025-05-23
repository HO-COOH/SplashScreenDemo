#include "Background.h"
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.h>
#include "Config.h"

Background::Background(
	winrt::Windows::UI::Composition::Compositor const& compositor, 
	winrt::Windows::Foundation::Numerics::float2 const& size,
	winrt::Windows::UI::Composition::VisualCollection const& visuals
) : SpriteVisual{ compositor.CreateSpriteVisual() }
{ 
	Size(size);
	Brush(compositor.CreateColorBrush(winrt::Windows::UI::Color{ .A = 0xFF, .R = Config::BackgroundColor >> 16, .G = Config::BackgroundColor >> 8, .B = Config::BackgroundColor & 0xFF }));
	visuals.InsertAtBottom(*this);
}

Background::Background(std::nullptr_t) : SpriteVisual{ nullptr }
{
}

void Background::StartToFade(winrt::Windows::UI::Composition::CompositionAnimation const& opacityAnimation)
{
	StartAnimation(
		L"Opacity",
		opacityAnimation
	);
}
