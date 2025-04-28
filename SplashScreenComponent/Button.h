#pragma once
#include <winrt/Windows.UI.Composition.h>

class Button : public winrt::Windows::UI::Composition::SpriteVisual
{
	winrt::Windows::UI::Composition::CompositionColorBrush m_backgroundBrush{ nullptr };
	winrt::Windows::UI::Composition::SpriteVisual m_content{ nullptr };
public:
	Button(
		winrt::Windows::UI::Composition::Compositor const& compositor,
		winrt::Windows::UI::Composition::SpriteVisual const& content
	);

	Button(std::nullptr_t) : SpriteVisual{ nullptr } {}

	void PlayColorAnimation(winrt::Windows::UI::Composition::ColorKeyFrameAnimation const& pointerOverColorAnimation);
};
