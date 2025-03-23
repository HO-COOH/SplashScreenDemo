#pragma once

#include <wil/com.h>

struct IWICImagingFactory;

class WICImagingFactory : public wil::com_ptr<IWICImagingFactory>
{
public:
	WICImagingFactory();
};
