#include "TextBlock.h"
#include "dwrite.h"
#include "DWriteFactory.h"
#include <winrt/base.h>
#include <d2d1.h>
#include <d2d1_2.h>
#include <windows.ui.composition.interop.h>


TextBlock::TextBlock(winrt::Windows::UI::Composition::Compositor const& compositor, winrt::Windows::UI::Composition::CompositionGraphicsDevice const& graphicsDevice, std::wstring_view text, float fontSize) : SpriteVisual{compositor.CreateSpriteVisual() }
{
	winrt::com_ptr<IDWriteTextFormat> textFormat;
	winrt::check_hresult(DWriteFactory::Instance->CreateTextFormat(
		TEXT("Segoe UI"),
		nullptr,
		DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_SEMI_LIGHT,
		DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		L"",
		textFormat.put()
	));
	textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING);
	Size({ 1000.f, 100.f });
	auto surface = graphicsDevice.CreateDrawingSurface({ 1000, 100 }, 
		winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized,
		winrt::Windows::Graphics::DirectX::DirectXAlphaMode::Premultiplied);
	auto surfaceInterop = surface.as<ABI::Windows::UI::Composition::ICompositionDrawingSurfaceInterop>();

	winrt::com_ptr<ID2D1DeviceContext> d2dContext;
	POINT offset;
	winrt::check_hresult(surfaceInterop->BeginDraw(nullptr, IID_PPV_ARGS(d2dContext.put()), &offset));
	winrt::com_ptr<ID2D1SolidColorBrush> textBrush;
	winrt::check_hresult(d2dContext->CreateSolidColorBrush(D2D1::ColorF{ D2D1::ColorF::Black }, textBrush.put()));
	d2dContext->DrawTextW(
		text.data(),
		text.size(),
		textFormat.get(),
		D2D1::RectF(0, 0, 1000, 100),
		textBrush.get()
	);
	winrt::check_hresult(surfaceInterop->EndDraw());

	Brush(compositor.CreateSurfaceBrush(surface));
}
