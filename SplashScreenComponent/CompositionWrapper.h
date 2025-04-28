#pragma once
#include <Windows.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Composition.Desktop.h>
#include <winrt/Windows.System.h>
#include <windows.ui.composition.interop.h>
#include "D3D11Device.hpp"
#include "DXGIFactory.hpp"
#include <d2d1_1.h>

class CompositionWrapper
{
	wil::com_ptr<ID2D1Device> d2dDevice;
	D3D11Device d3d11Device;
	DXGIFactory dxgiFactory;
	//winrt::Windows::UI::Composition::CompositionGraphicsDevice graphicsDevice{ nullptr };
	wil::com_ptr<IDXGIDevice> dxgiDevice;
public:
	winrt::Windows::System::DispatcherQueueController m_dispatcherQueueController;
	winrt::Windows::UI::Composition::Compositor m_compositor;
	winrt::Windows::UI::Composition::Desktop::DesktopWindowTarget m_target{ nullptr };
	winrt::Windows::UI::Composition::VisualCollection visuals{ nullptr };


	CompositionWrapper(HWND hwnd);

	static auto GetInterop(winrt::Windows::UI::Composition::Compositor const& compositor)
	{
		return compositor.as<ABI::Windows::UI::Composition::ICompositorInterop>();
	}

	winrt::Windows::UI::Composition::CompositionGraphicsDevice GetGraphicsDevice();
};

