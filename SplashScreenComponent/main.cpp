#include <Windows.h>

//WIP
class SplashWindow
{
	HWND m_hwnd;

	constexpr static auto className = L"SplashWindowClass";
	static void registerClassIfNeeded()
	{
		static bool registered = false;
		
		if (registered)
			return;

		WNDCLASS const windowClass
		{
			//.style = CS_HREDRAW | CS_VREDRAW,
			.lpfnWndProc = windowProc,
			.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH),
			.lpszClassName = className
		};
		RegisterClass(&windowClass);
		registered = true;
	}

	static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	void createWindow()
	{
		m_hwnd = CreateWindow(
			className,
			L"SplashWindow",
			WS_OVERLAPPEDWINDOW,
			0,
			0,
			800,
			600,
			nullptr,
			nullptr,
			nullptr,
			nullptr
		);

		ShowWindow(m_hwnd, SW_SHOW);
	}
public:
	SplashWindow()
	{
		registerClassIfNeeded();

		createWindow();
	}
};

int WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
)
{
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
}
