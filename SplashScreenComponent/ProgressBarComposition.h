#pragma once
#include <winrt/Windows.UI.Composition.h>

namespace winrt::Windows::Foundation::Numerics
{
	struct float2;
}

class ProgressBarComposition : public winrt::Windows::UI::Composition::ShapeVisual
{
public:
	ProgressBarComposition(
		winrt::Windows::UI::Composition::Compositor const& compositor,
		winrt::Windows::Foundation::Numerics::float2 const& size,
		winrt::Windows::UI::Composition::VisualCollection const& visuals
	);

	ProgressBarComposition(std::nullptr_t) : ShapeVisual{ nullptr } {}

	void StartToFade(winrt::Windows::UI::Composition::CompositionAnimation const& opacityAnimation);
};
