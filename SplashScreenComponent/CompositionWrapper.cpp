#include "CompositionWrapper.h"
#include <DispatcherQueue.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.Xaml.Media.h>
#include <d2d1_1.h>
#include <d2d1_2.h>
#include "D2D1Factory.h"

CompositionWrapper::CompositionWrapper(HWND hwnd) :
	m_dispatcherQueueController{ [] {
		DispatcherQueueOptions options
		{
			.dwSize = sizeof(DispatcherQueueOptions),
			.threadType = DISPATCHERQUEUE_THREAD_TYPE::DQTYPE_THREAD_CURRENT,
			.apartmentType = DISPATCHERQUEUE_THREAD_APARTMENTTYPE::DQTAT_COM_STA,
		};
		ABI::Windows::System::IDispatcherQueueController* controller;
		CreateDispatcherQueueController(options, &controller);
		return winrt::Windows::System::DispatcherQueueController{ controller, winrt::take_ownership_from_abi };
	}() }
{
	winrt::check_hresult(m_compositor.as<ABI::Windows::UI::Composition::Desktop::ICompositorDesktopInterop>()->CreateDesktopWindowTarget(
		hwnd,
		true,
		reinterpret_cast<ABI::Windows::UI::Composition::Desktop::IDesktopWindowTarget**>(winrt::put_abi(m_target))
	));

	auto containerVisual = m_compositor.CreateContainerVisual();
	visuals = containerVisual.Children();
	m_target.Root(containerVisual);
}

winrt::Windows::UI::Composition::CompositionGraphicsDevice CompositionWrapper::GetGraphicsDevice()
{
	//if (graphicsDevice)
	//	return graphicsDevice;

	if (!dxgiDevice)
	{
		winrt::check_hresult(d3d11Device->QueryInterface(dxgiDevice.put()));
		winrt::check_hresult(D2D1Factory::Instance->CreateDevice(dxgiDevice.get(), d2dDevice.put()));
	}

	winrt::Windows::UI::Composition::CompositionGraphicsDevice graphicsDevice{ nullptr };
	GetInterop(m_compositor)->CreateGraphicsDevice(
		d2dDevice.get(),
		reinterpret_cast<ABI::Windows::UI::Composition::ICompositionGraphicsDevice**>(winrt::put_abi(graphicsDevice))
	);
	return graphicsDevice;
}
