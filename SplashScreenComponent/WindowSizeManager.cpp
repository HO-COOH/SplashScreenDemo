#include "WindowSizeManager.h"
#include <winrt/base.h>

SIZE WindowSizeManager::GetWindowSize(UINT dpi)
{
    RECT rect{ .right = 800,.bottom = 600 };
    winrt::check_bool(AdjustWindowRectExForDpi(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_NOREDIRECTIONBITMAP, dpi));
    return { rect.right - rect.left, rect.bottom - rect.top };
}

SIZE WindowSizeManager::GetWindowClientSize()
{
    return { 800, 600 };
}
