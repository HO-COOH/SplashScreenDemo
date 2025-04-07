#include "D2D1Factory.h"
#include <wil/result.h>
#include <d2d1_2.h>

#pragma comment(lib, "d2d1.lib")

D2D1Factory::D2D1Factory()
{
	THROW_IF_FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED, put()));
}

D2D1Factory D2D1Factory::Instance;