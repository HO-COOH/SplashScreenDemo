#include "TextBlock.h"
#include "dwrite.h"
#include "DWriteFactory.h"
#include <winrt/base.h>
#include <d2d1.h>

TextBlock::TextBlock(winrt::Windows::UI::Composition::Compositor const& compositor, winrt::Windows::UI::Composition::CompositionGraphicsDevice& graphicsDevice, std::wstring_view text, float fontSize) : SpriteVisual{compositor.CreateSpriteVisual() }
{
	winrt::com_ptr<IDWriteTextFormat> textFormat;
	winrt::check_hresult(DWriteFactory::Instance->CreateTextFormat(
		TEXT("Segoe UI"),
		nullptr,
		DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		L"",
		textFormat.put()
	));

	auto surface = graphicsDevice.CreateDrawingSurface({ 1000, 100 }, 
		winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized,
		winrt::Windows::Graphics::DirectX::DirectXAlphaMode::Premultiplied);
	
	winrt::com_ptr<ID2D1SolidColorBrush> textBrush;

}
