#pragma once
#include <wil/com.h>
#include <wil/result.h>
#include <d3d11.h>


class D3D11Device : public wil::com_ptr<ID3D11Device>
{
public:
	D3D11Device()
	{
		THROW_IF_FAILED(
			D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_BGRA_SUPPORT,
				nullptr,
				0,
				D3D11_SDK_VERSION,
				put(),
				nullptr,
				nullptr
			)
		);
	} 
};
