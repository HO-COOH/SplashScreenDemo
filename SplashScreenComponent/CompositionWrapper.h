#pragma once
#include <Windows.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Composition.Desktop.h>
#include <winrt/Windows.System.h>
#include <windows.ui.composition.interop.h>

struct CompositionWrapper
{
	winrt::Windows::System::DispatcherQueueController m_dispatcherQueueController;
	winrt::Windows::UI::Composition::Compositor m_compositor;
	winrt::Windows::UI::Composition::Desktop::DesktopWindowTarget m_target{ nullptr };
	winrt::Windows::UI::Composition::VisualCollection visuals{ nullptr };

	CompositionWrapper(HWND hwnd);

	auto GetInterop()
	{
		return m_compositor.as<ABI::Windows::UI::Composition::ICompositorInterop>();
	}
};

