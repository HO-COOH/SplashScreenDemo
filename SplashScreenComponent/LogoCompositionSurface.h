#pragma once

#include <winrt/Windows.UI.Composition.h>

#include <windows.ui.composition.interop.h>
#include <Windows.h>

struct LogoCompositionSurface : winrt::Windows::UI::Composition::SpriteVisual
{
public:
	LogoCompositionSurface(
		winrt::Windows::UI::Composition::Compositor const& compositor,
		winrt::Windows::UI::Composition::CompositionGraphicsDevice const& graphicsDevice, 
		UINT scaledWidth, UINT scaledHeight);

	LogoCompositionSurface(std::nullptr_t) : SpriteVisual{ nullptr } {}
};
