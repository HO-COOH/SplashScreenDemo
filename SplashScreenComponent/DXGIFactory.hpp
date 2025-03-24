#pragma once
#include <wil/com.h>
#include <wil/result.h>
#include <dxgi1_3.h>
#include <d3d11_2.h>
#pragma comment(lib, "dxgi.lib")

class DXGIFactory : private wil::com_ptr<IDXGIFactory2>
{
public:
	DXGIFactory()
	{
		THROW_IF_FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(put())));
	}

	auto CreateSwapChainForComposition(UINT width, UINT height, IDXGIDevice* dxgiDevice)
	{
		DXGI_SWAP_CHAIN_DESC1 description
		{
			.Width = width,
			.Height = height,
			.Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
			.SampleDesc = DXGI_SAMPLE_DESC
			{
				.Count = 1 
			},	
			.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
			.BufferCount = 2,
			.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
			.AlphaMode = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_PREMULTIPLIED
		};

		wil::com_ptr<IDXGISwapChain1> swapChain;
		THROW_IF_FAILED((*this)->CreateSwapChainForComposition(
			dxgiDevice,
			&description,
			nullptr,
			swapChain.put()
		));
		return swapChain;
	}
};
