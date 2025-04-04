#include "Logo.h"
#include <wincodec.h>
#include <wil/com.h>
#include <wil/result.h>
#include <d2d1.h>
#include <optional>
#include "Config.h"
#include "WICImagingFactory.h"

static std::optional<WICImagingFactory> wicImagingFactory;

Logo::Logo(std::wstring_view filePath, ID2D1RenderTarget* renderTarget)
{
	wicImagingFactory.emplace();

	wil::com_ptr<IWICBitmapDecoder> decoder;
	wil::com_ptr<IWICBitmapFrameDecode> frame;
	THROW_IF_FAILED((*wicImagingFactory)->CreateDecoderFromFilename(
		filePath.data(),
		nullptr,
		GENERIC_READ,
		WICDecodeOptions::WICDecodeMetadataCacheOnDemand,
		decoder.put()));
	THROW_IF_FAILED(decoder->GetFrame(0, frame.put()));
	//convert format to 32bppPBGRA
	wil::com_ptr<IWICFormatConverter> converter;
	THROW_IF_FAILED((*wicImagingFactory)->CreateFormatConverter(converter.put()));
	THROW_IF_FAILED(converter->Initialize(
		frame.get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherType::WICBitmapDitherTypeNone,
		nullptr,
		0.f,
		WICBitmapPaletteType::WICBitmapPaletteTypeCustom));

	THROW_IF_FAILED(renderTarget->CreateBitmapFromWicBitmap(
		converter.get(),
		bitmap.put()
	));
}

void Logo::OnPaint(ID2D1RenderTarget* renderTarget, FLOAT opacity)
{
	D2D1_RECT_F bitmapDestination
	{
		.left = (m_width - Config::LogoWidth) / 2.f,
		.top = (m_height - Config::LogoHeight) / 2.f
	};
	bitmapDestination.right = bitmapDestination.left + Config::LogoWidth;
	bitmapDestination.bottom = bitmapDestination.top + Config::LogoHeight;

	renderTarget->DrawBitmap(bitmap.get(), &bitmapDestination, opacity);
}

