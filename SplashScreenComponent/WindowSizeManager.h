#pragma once
#include <Windows.h>

//Keep track of the main project window size, so it's more like UWP
//TODO: Not implemented yet
class WindowSizeManager
{
public:
	static SIZE GetWindowSize(UINT dpi = 96);
	static SIZE GetWindowClientSize();
};
