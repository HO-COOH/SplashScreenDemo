#pragma once
#include <winrt/Windows.UI.Composition.h>

namespace winrt::Windows::Foundation::Numerics
{
	struct float2;
}

class Background : public winrt::Windows::UI::Composition::SpriteVisual
{
public:
	Background(
		winrt::Windows::UI::Composition::Compositor const& compositor,
		winrt::Windows::Foundation::Numerics::float2 const& size,
		winrt::Windows::UI::Composition::VisualCollection const& visuals
	);

	Background(std::nullptr_t);

	void StartToFade(winrt::Windows::UI::Composition::CompositionAnimation const& opacityAnimation);
};
