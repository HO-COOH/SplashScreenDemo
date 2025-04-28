#pragma once
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.Foundation.h>

class LoadedSvgSurface : public winrt::Windows::UI::Composition::CompositionDrawingSurface
{
public:
	LoadedSvgSurface(
		char const* svg,
		winrt::Windows::Foundation::Size size,
		winrt::Windows::UI::Composition::CompositionGraphicsDevice const& graphicsDevice
	);

	LoadedSvgSurface(std::nullptr_t) : CompositionDrawingSurface{ nullptr } {}
};
