#pragma once
#include <wil/com.h>


struct ID2D1Factory;

struct D2D1Factory : public wil::com_ptr<ID2D1Factory>
{
	D2D1Factory();
};
