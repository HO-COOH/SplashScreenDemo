#pragma once
#include <wil/com.h>
#include <wil/result.h>
#include <dxgi1_3.h>

class DXGIFactory : public wil::com_ptr<IDXGIFactory2>
{
public:
	DXGIFactory()
	{
		THROW_IF_FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(put())));
	}

	auto CreateSwapChainForComposition(UINT width, UINT height)
	{
		DXGI_SWAP_CHAIN_DESC1 description
		{
			.Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
		};
	}
};
