#pragma once
#include <d2d1.h>
#include <type_traits>
#include <d2d1_3.h>
#include <winrt/base.h>

struct ID2D1DeviceContext5;
struct ID2D1DeviceContext;

void DrawSvg(char const* svg, D2D1_SIZE_F size, ID2D1DeviceContext5* deviceContext);

template<typename IDeviceContextType, typename... Args>
void DrawSvg(char const* svg, D2D1_SIZE_F size, IDeviceContextType* deviceContext, Args&&... args)
{
	if constexpr (std::is_same_v<IDeviceContextType, ID2D1DeviceContext>)
	{
		winrt::com_ptr<ID2D1DeviceContext5> deviceContext5;
		winrt::check_hresult(deviceContext->QueryInterface(deviceContext5.put()));
		DrawSvg(svg, size, deviceContext5.get(), args...);
	}
	else if constexpr(std::is_same_v<IDeviceContextType, ID2D1DeviceContext5>)
	{
		DrawSvg(svg, size, deviceContext, args...);
	}
	else
	{
		static_assert(false);
	}
}

void DrawSvg(char const* svg, D2D1_SIZE_F size, ID2D1DeviceContext5* deviceContext, POINT offset);
