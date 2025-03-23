#pragma once

#include "MainWindow.g.h"
#include "StartupTimer.h"
#include "SplashScreenComponentMessageQueue.h"

namespace winrt::SplashScreenDemo::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow()
        {
            StartupTimer::GetInstance().SetBeforeMainWindowConstructed();
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void myButton_Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);

        void Window_Activated(
            winrt::Windows::Foundation::IInspectable const&, 
            winrt::Microsoft::UI::Xaml::WindowActivatedEventArgs const&)
        {
            static bool shown = false;
            if (!shown)
            {
                shown = true;
                StartupTimer::GetInstance().SetWindowShown();
                StartupTimer::GetInstance().Print();
                //ExitProcess(0);
            }
        }
    };
}

namespace winrt::SplashScreenDemo::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
