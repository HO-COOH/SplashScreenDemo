#pragma once
#include <wil/com.h>
#include "ComponentBase.h"

#pragma region Forward declaration
struct ID2D1Factory;
struct ID2D1RenderTarget;
struct ID2D1RoundedRectangleGeometry;
struct ID2D1SolidColorBrush;
#pragma endregion

class ProgressBar : public ComponentBase
{
	wil::com_ptr<ID2D1RoundedRectangleGeometry> clipGeometry;
	wil::com_ptr<ID2D1SolidColorBrush> progressBarFillBrush;
	float m_translation{};
public:
	ProgressBar(ID2D1RenderTarget* renderTarget);
	void OnPaint(ID2D1RenderTarget* renderTarget);
	void OnSize(UINT width, UINT height, ID2D1Factory* d2d1Factory);

	void OnTick();
};
