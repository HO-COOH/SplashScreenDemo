#include "ProgressBarComposition.h"
#include <DispatcherQueue.h>

#include "Config.h"
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.Xaml.Media.h>


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
	}()}
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
