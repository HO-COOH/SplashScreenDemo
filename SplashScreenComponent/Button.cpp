#include "Button.h"
#include "CompositionWrapper.h"
#include "Config.h"

Button::Button(
	winrt::Windows::UI::Composition::Compositor const& compositor, 
	winrt::Windows::UI::Composition::SpriteVisual const& content,
	winrt::Windows::Foundation::Numerics::float2 size) :
	m_backgroundBrush{compositor.CreateColorBrush(winrt::Windows::UI::Colors::Transparent())},
	SpriteVisual{compositor.CreateSpriteVisual()}
{
	Size(size);
	Brush(m_backgroundBrush);

	content.AnchorPoint({ 0.5f, 0.5f });
	content.RelativeOffsetAdjustment({ 0.5f, 0.5f, 0 });
	
	Children().InsertAtTop(content);
}

void Button::PlayColorAnimation(winrt::Windows::UI::Composition::ColorKeyFrameAnimation const& pointerOverColorAnimation)
{
	m_backgroundBrush.StartAnimation(L"Color", pointerOverColorAnimation);
}

void Button::SetContent(winrt::Windows::UI::Composition::SpriteVisual const& content)
{
	auto children = Children();
	children.RemoveAll();
	content.AnchorPoint({ 0.5f, 0.5f });
	content.RelativeOffsetAdjustment({ 0.5f, 0.5f, 0 });

	children.InsertAtTop(content);
}
