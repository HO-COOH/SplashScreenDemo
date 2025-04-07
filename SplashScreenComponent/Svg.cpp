#include "Svg.h"
#include <string_view>
#include <Shlwapi.h>
#include <d2d1svg.h>
#include <wil/com.h>
#include <d2d1_3.h>
#include <winrt/base.h>
#pragma comment(lib, "shlwapi.lib")

static auto CreateStreamFromStringAlt(const std::string_view content)
{
	return SHCreateMemStream(
		reinterpret_cast<BYTE const*>(content.data()),
		content.size()
	);
}

void DrawSvg(char const* svgString, ID2D1DeviceContext5* deviceContext)
{
	winrt::com_ptr<ID2D1SvgDocument> svg;
	winrt::check_hresult(deviceContext->CreateSvgDocument(
		CreateStreamFromStringAlt(svgString),
		{ 200.f, 200.f },
		svg.put()
	));
	deviceContext->DrawSvgDocument(svg.get());
}
