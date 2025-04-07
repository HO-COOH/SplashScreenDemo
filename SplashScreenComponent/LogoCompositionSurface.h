#pragma once
#include <wil/com.h>
#include "D3D11Device.hpp"
#include "DXGIFactory.hpp"
#include <optional>
#include <winrt/Windows.UI.Composition.h>
#include "Logo.h"
#include <windows.ui.composition.interop.h>
#include <Windows.h>

struct ID2D1Device;

struct LogoCompositionSurface
{
	wil::com_ptr<ID2D1Device> d2dDevice;
	std::optional<Logo> m_logo;

	D3D11Device d3d11Device;
	DXGIFactory dxgiFactory;

	winrt::Windows::UI::Composition::CompositionDrawingSurface m_drawingSurface{ nullptr };
public:
	LogoCompositionSurface(ABI::Windows::UI::Composition::ICompositorInterop* compositorInterop, UINT scaledWidth, UINT scaledHeight);
};
