#include "LogoCompositionSurface.h"
#include <optional>
#include "D2D1Factory.h"
#include "Config.h"
#include <d2d1_1.h>
#include "Logo.h"
#include <filesystem>

static auto GetAsset(wchar_t const* fileName)
{
	std::array<wchar_t, MAX_PATH> buffer{};
	GetModuleFileName(nullptr, buffer.data(), std::size(buffer));
	return std::filesystem::path{ buffer.data() }
		.parent_path()
		.parent_path()
		.parent_path() 
		/ L"SplashScreenDemo (Package)" 
		/ L"Images" 
		/ fileName;
}

LogoCompositionSurface::LogoCompositionSurface(winrt::Windows::UI::Composition::Compositor const& compositor, winrt::Windows::UI::Composition::CompositionGraphicsDevice const& graphicsDevice, UINT scaledWidth, UINT scaledHeight) : SpriteVisual{compositor.CreateSpriteVisual() }
{

	winrt::Windows::UI::Composition::CompositionDrawingSurface m_drawingSurface = graphicsDevice.CreateDrawingSurface(
		{
			static_cast<float>(scaledWidth),
			static_cast<float>(scaledHeight)
		},
		winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized,
		winrt::Windows::Graphics::DirectX::DirectXAlphaMode::Premultiplied
	);
	auto m_drawingSurfaceInterop = m_drawingSurface.as<ABI::Windows::UI::Composition::ICompositionDrawingSurfaceInterop>();
	wil::com_ptr<ID2D1DeviceContext> d2dContext;

	POINT offset;
	m_drawingSurfaceInterop->BeginDraw(
		nullptr,
		IID_PPV_ARGS(d2dContext.put()),
		&offset
	);

	Logo m_logo{ GetAsset(Config::LogoFile).wstring(), d2dContext.get()};
	scaledWidth += offset.x;
	scaledHeight += offset.y;

	m_logo.OnSize(scaledWidth, scaledHeight);
	m_logo.OnPaint(d2dContext.get());

	winrt::check_hresult(m_drawingSurfaceInterop->EndDraw());

	Size({ static_cast<float>(scaledWidth), static_cast<float>(scaledHeight) });
	Brush(compositor.CreateSurfaceBrush(m_drawingSurface));
}