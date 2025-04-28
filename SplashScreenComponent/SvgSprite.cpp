#include "SvgSprite.h"
#include "LoadedSvgSurface.h"

SvgSprite::SvgSprite(
	char const* svg, 
	winrt::Windows::Foundation::Size const& size,
	winrt::Windows::UI::Composition::Compositor const& compositor, 
	winrt::Windows::UI::Composition::CompositionGraphicsDevice const& graphicsDevice)
	: SpriteVisual{compositor.CreateSpriteVisual() }
{
	Size(size);
	Brush(compositor.CreateSurfaceBrush(LoadedSvgSurface{ svg, size, graphicsDevice }));
}
