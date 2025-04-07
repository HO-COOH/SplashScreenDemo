#include "LogoCompositionSurface.h"
#include <d2d1_1.h>
#include <d2d1_2.h>
#include "D2D1Factory.h"
#include "Config.h"

LogoCompositionSurface::LogoCompositionSurface(ABI::Windows::UI::Composition::ICompositorInterop* compositorInterop, UINT scaledWidth, UINT scaledHeight)
{
	wil::com_ptr<IDXGIDevice> dxgiDevice;

	winrt::check_hresult(d3d11Device->QueryInterface(dxgiDevice.put()));
	winrt::check_hresult(D2D1Factory::Instance->CreateDevice(dxgiDevice.get(), d2dDevice.put()));

	winrt::Windows::UI::Composition::CompositionGraphicsDevice graphicsDevice{ nullptr };
	winrt::check_hresult(compositorInterop->CreateGraphicsDevice(
		d2dDevice.get(),
		reinterpret_cast<ABI::Windows::UI::Composition::ICompositionGraphicsDevice**>(winrt::put_abi(graphicsDevice))));
	m_drawingSurface = graphicsDevice.CreateDrawingSurface(
		{
			static_cast<float>(scaledWidth),
			static_cast<float>(scaledHeight)
		},
		winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized,
		winrt::Windows::Graphics::DirectX::DirectXAlphaMode::Premultiplied
	);
	auto m_drawingSurfaceInterop = m_drawingSurface.as<ABI::Windows::UI::Composition::ICompositionDrawingSurfaceInterop>();
	wil::com_ptr<ID2D1DeviceContext> d2dContext;

	POINT offset;
	m_drawingSurfaceInterop->BeginDraw(
		nullptr,
		IID_PPV_ARGS(d2dContext.put()),
		&offset
	);

	m_logo.emplace(Config::LogoPath, d2dContext.get());
	m_logo->OnSize(scaledWidth, scaledHeight);
	m_logo->OnPaint(d2dContext.get());

	winrt::check_hresult(m_drawingSurfaceInterop->EndDraw());
}