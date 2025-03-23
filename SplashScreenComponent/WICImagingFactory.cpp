#include "WICImagingFactory.h"
#include <wil/result.h>
#include <wincodec.h>

WICImagingFactory::WICImagingFactory()
{
	THROW_IF_FAILED(CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(put())
	));
}
