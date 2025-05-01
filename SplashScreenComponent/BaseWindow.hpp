#pragma once
#include <wil/resource.h>
#include <string_view>

template<size_t N>
struct StringLiteral {
	constexpr StringLiteral(const wchar_t(&str)[N]) {
		std::copy_n(str, N, value);
	}

	wchar_t value[N];
};

template<typename T, StringLiteral className>
class BaseWindow
{
	static void OnPaint(HWND) 
	{
	}
protected:
	wil::unique_hwnd m_hwnd;

	[[nodiscard]] static auto getSelfFromHwnd(HWND const window)
	{
		return reinterpret_cast<T*>(GetWindowLongPtr(window, GWLP_USERDATA));
	}

	static void OnNCCreate(HWND hwnd, CREATESTRUCT* lparam)
	{
	}

	static void OnSize(HWND hwnd, WPARAM wparam, UINT width, UINT height)
	{
	}

	static void OnDpiChanged(HWND hwnd, WORD dpiX, WORD dpiY, RECT* suggestPosition)
	{
		SetWindowPos(
			hwnd,
			nullptr,
			suggestPosition->left,
			suggestPosition->top,
			suggestPosition->right - suggestPosition->left,
			suggestPosition->bottom - suggestPosition->top,
			SWP_NOZORDER | SWP_NOACTIVATE
		);
	}

	static void OnActivate(HWND hwnd, WPARAM wparam, LPARAM lparam)
	{
	}

	static void OnWindowPosChanging(HWND hwnd, WINDOWPOS* windowPos)
	{
	}

	static LRESULT OnUserMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	//static LRESULT OnMouseMove(HWND hwnd, WPARAM buttonDown, WORD x, WORD y)
	//{
	//	return DefWindowProc(hwnd, WM_MOUSEMOVE, buttonDown, MAKELONG(x, y));
	//}

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
			//Methods that have default (no-op) implementation
			case WM_DPICHANGED:
				T::OnDpiChanged(hwnd, LOWORD(wparam), HIWORD(wparam), reinterpret_cast<RECT*>(lparam));
				break;
			case WM_NCCREATE:
				OnNCCreate(hwnd, reinterpret_cast<CREATESTRUCT*>(lparam));
				break;
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
			case WM_WINDOWPOSCHANGING:
				OnWindowPosChanging(hwnd, reinterpret_cast<WINDOWPOS*>(lparam));
				break;
				//Methods that must be implemented by derived class 
			case WM_PAINT:
				T::OnPaint(hwnd);
				break;
			case WM_SIZE:
				T::OnSize(hwnd, wparam, LOWORD(lparam), HIWORD(lparam));
				return 0;
			case WM_MOUSEMOVE:
				T::OnMouseMove(hwnd, wparam, LOWORD(lparam), HIWORD(lparam));
				return 0;
			default:
				return T::OnUserMessage(hwnd, msg, wparam, lparam);
		}
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}

public:
	BaseWindow(std::wstring_view windowName, DWORD exStyle = 0, DWORD style = WS_OVERLAPPEDWINDOW, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT)
	{
		static auto windowClass = [] {
			WNDCLASS wc
			{
				.style = CS_HREDRAW | CS_VREDRAW,
				.lpfnWndProc = WndProc,
				.cbWndExtra = sizeof(void*),
				.lpszClassName = className.value,
			};
			RegisterClass(&wc);
			return wc;
			}();

		auto const hwnd = CreateWindowExW(
			exStyle,
			className.value,
			windowName.data(),
			style,
			x,
			y,
			cx,
			cy,
			nullptr,
			nullptr,
			nullptr,
			static_cast<T*>(this)
		);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(static_cast<T*>(this)));

		ShowWindow(hwnd, SW_SHOW);
		m_hwnd.reset(hwnd);
	}

	RECT ClientRect() const
	{
		RECT clientRect;
		THROW_IF_WIN32_BOOL_FALSE(GetClientRect(m_hwnd.get(), &clientRect));
		return clientRect;
	}

	UINT GetDpi() const
	{
		return GetDpiForWindow(m_hwnd.get());
	}
};
