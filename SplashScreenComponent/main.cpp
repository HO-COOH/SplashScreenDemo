#include <Windows.h>
#include "StartupTimer.h"


#include <wil/result_macros.h>
#include "SplashWindow.h"






LPSTR argv{};


int WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
)
{
	THROW_IF_FAILED(CoInitialize(nullptr));
	StartupTimer::GetInstance();
	argv = lpCmdLine;

	SplashWindow window;

	MSG msg;
	BOOL ret;
	while ((ret = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (ret == -1)
		{
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	SetForegroundWindow(SplashWindow::syncMoveWithWindow);
}
