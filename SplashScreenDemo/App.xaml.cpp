#include "pch.h"
#include "App.xaml.h"
#include "MainWindow.xaml.h"
#include "StartupTimer.h"
#include "HwndHelper.hpp"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::SplashScreenDemo::implementation
{
    /// <summary>
    /// Initializes the singleton application object.  This is the first line of authored code
    /// executed, and as such is the logical equivalent of main() or WinMain().
    /// </summary>
    App::App()
    {
        // Xaml objects should not call InitializeComponent during construction.
        // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
        UnhandledException([](IInspectable const&, UnhandledExceptionEventArgs const& e)
        {
            if (IsDebuggerPresent())
            {
                auto errorMessage = e.Message();
                __debugbreak();
            }
        });
#endif
    }

    /// <summary>
    /// Invoked when the application is launched.
    /// </summary>
    /// <param name="e">Details about the launch request and process.</param>
    void App::OnLaunched([[maybe_unused]] LaunchActivatedEventArgs const& e)
    {
        StartupTimer::GetInstance().SetAppLaunch();
        window = make<MainWindow>();

        auto cmd = GetCommandLine();
        int argc{};
        auto argv = CommandLineToArgvW(cmd, &argc);
        if (argc > 1 && std::wstring_view{ argv[1] }.starts_with(L"SplashScreenDemo_"))
        {
            SplashScreenComponentMessageQueue queue{ argv[1] };
            auto hwnd = queue.splashScreenHwnd();
            //Get hwnd size
			RECT rect;
			GetWindowRect(hwnd, &rect);
            //set window position

            auto mainHwnd = GetHwnd(window);
			SetWindowPos(
                mainHwnd, 
                nullptr, 
                rect.left, 
                rect.top, 
                rect.right - rect.left, 
                rect.bottom - rect.top, 
                SWP_NOZORDER | SWP_NOACTIVATE);
            queue.SendHwnd(mainHwnd);
        }
        window.Activate();
    }
}

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    StartupTimer::GetInstance();

    winrt::init_apartment(winrt::apartment_type::single_threaded);
    ::winrt::Microsoft::UI::Xaml::Application::Start(
        [](auto&&)
        {
            ::winrt::make<::winrt::SplashScreenDemo::implementation::App>();
        });

    return 0;
}