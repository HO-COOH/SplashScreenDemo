#pragma once

//Keep track of the main project window size, so it's more like UWP
//TODO: Not implemented yet
class WindowSizeManager
{
public:
	struct Size
	{
		unsigned int width;
		unsigned int height;
	};

	static Size GetWindowSize();
};
