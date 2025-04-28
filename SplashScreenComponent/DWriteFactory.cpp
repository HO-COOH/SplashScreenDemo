#include "DWriteFactory.h"
#include <dwrite.h>

DWriteFactory DWriteFactory::Instance;

DWriteFactory::DWriteFactory()
{
	winrt::check_hresult(DWriteCreateFactory(
		DWRITE_FACTORY_TYPE::DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(put())
	));
}
