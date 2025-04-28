#include "Button.h"
#include "CompositionWrapper.h"
#include "Config.h"

Button::Button(winrt::Windows::UI::Composition::Compositor const& compositor, winrt::Windows::UI::Composition::SpriteVisual const& content) :
	m_backgroundBrush{compositor.CreateColorBrush(winrt::Windows::UI::Colors::Red())},
	m_content{content},
	SpriteVisual{compositor.CreateSpriteVisual()}
{
	Size({ Config::CaptionButtonWidth, Config::CaptionButtonHeight });
	Brush(m_backgroundBrush);

	content.AnchorPoint({ 0.5f, 0.5f });
	content.RelativeOffsetAdjustment({ 0.5f, 0.5f, 0 });
	
	Children().InsertAtTop(m_content);
}

void Button::PlayColorAnimation(winrt::Windows::UI::Composition::ColorKeyFrameAnimation const& pointerOverColorAnimation)
{
	m_backgroundBrush.StartAnimation(L"Color", pointerOverColorAnimation);
}
