#pragma once
#include <wil/com.h>
#include <string_view>
#include "ComponentBase.h"

#pragma region Forward declaration
struct ID2D1Bitmap;
struct ID2D1RenderTarget;
#pragma endregion

class Logo : public ComponentBase
{
	wil::com_ptr<ID2D1Bitmap> bitmap;
public:
	Logo(std::wstring_view filePath, ID2D1RenderTarget* renderTarget);
	void OnPaint(ID2D1RenderTarget* renderTarget);
};
