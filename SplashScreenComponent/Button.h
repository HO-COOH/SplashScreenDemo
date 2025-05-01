#pragma once
#include <winrt/Windows.UI.Composition.h>

class Button : public winrt::Windows::UI::Composition::SpriteVisual
{
protected:
	winrt::Windows::UI::Composition::CompositionColorBrush m_backgroundBrush{ nullptr };
public:
	Button(
		winrt::Windows::UI::Composition::Compositor const& compositor,
		winrt::Windows::UI::Composition::SpriteVisual const& content,
		winrt::Windows::Foundation::Numerics::float2 size
	);

	Button(std::nullptr_t) : SpriteVisual{ nullptr } {}

	void PlayColorAnimation(winrt::Windows::UI::Composition::ColorKeyFrameAnimation const& pointerOverColorAnimation);

	void SetContent(winrt::Windows::UI::Composition::SpriteVisual const& content);
};
