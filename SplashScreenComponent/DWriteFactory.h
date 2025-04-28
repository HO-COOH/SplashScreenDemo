#pragma once

#include <winrt/base.h>

struct IDWriteFactory;

struct DWriteFactory : public winrt::com_ptr<IDWriteFactory>
{
	DWriteFactory();

	static DWriteFactory Instance;
};
