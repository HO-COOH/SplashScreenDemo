#include "LoadedSvgSurface.h"
#include <winrt/Windows.Foundation.h>
#include <d2d1_2.h>
#include <windows.ui.composition.interop.h>
#include "Svg.h"

LoadedSvgSurface::LoadedSvgSurface(
	char const* svg, 
	winrt::Windows::Foundation::Size size,
	winrt::Windows::UI::Composition::CompositionGraphicsDevice const& graphicsDevice)
	: CompositionDrawingSurface{graphicsDevice.CreateDrawingSurface(
		{size.Width, size.Height},
			winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized,
			winrt::Windows::Graphics::DirectX::DirectXAlphaMode::Premultiplied
		)
	}
{
	auto drawingSurfaceInterop = as<ABI::Windows::UI::Composition::ICompositionDrawingSurfaceInterop>();
	winrt::com_ptr<ID2D1DeviceContext> d2dContext;
	POINT offset{};
	auto hr = drawingSurfaceInterop->BeginDraw(nullptr, IID_PPV_ARGS(d2dContext.put()), &offset);
	DrawSvg(svg, { size.Width, size.Height }, d2dContext.get(), POINT{ 1, 2 });
	winrt::check_hresult(drawingSurfaceInterop->EndDraw());
}
