#pragma once
#include <Windows.h>
#include <winrt/Windows.Foundation.Numerics.h>

template<typename Ret = float>
auto ScaleForDpi(auto value, UINT dpi)
{
	return static_cast<Ret>(value * dpi / 96);
};

inline auto ScaleForDpi(winrt::Windows::Foundation::Numerics::float3 value, UINT dpi)
{
	return winrt::Windows::Foundation::Numerics::float3{
		ScaleForDpi(value.x, dpi),
		ScaleForDpi(value.y, dpi),
		ScaleForDpi(value.z, dpi)
	};
}
