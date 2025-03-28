﻿#include <Windows.h>
#include "StartupTimer.h"
#include <optional>
#include "MainApplicationLoadingMessageQueue.h"
#include "D2D1Factory.h"
#include <d2d1.h>
#include <d2d1_2.h>
#include <wil/com.h>
#include <wincodec.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include "WICImagingFactory.h"
#include "DXGIFactory.hpp"
#include "D3D11Device.hpp"
#include "dxgi.h"
#include <dcomp.h>
#pragma comment(lib, "dcomp.lib")

static LPSTR argv{};
static HWND syncMoveWithWindow{};
static std::optional<MainApplicationLoadingMessageQueue> messageQueue;
static D2D1Factory d2d1Factory;
static std::optional<WICImagingFactory> wicImagingFactory;

namespace Config
{
	static inline auto ProgressBarFillColor = D2D1::ColorF(0x0000FF);
	constexpr static auto ProgressBarWidth = 300.f;
	constexpr static auto ProgressBarHeight = 5.f;
	constexpr static auto ProgressBarRadius = 4.f;
	constexpr static auto ProgressBarMarginBottom = 100.f;
	constexpr static auto LogoWidth = 300;
	constexpr static auto LogoHeight = 300;
}

//WIP
class SplashWindow
{
	HWND m_hwnd;
	//wil::com_ptr<ID2D1HwndRenderTarget> m_renderTarget;
	wil::com_ptr<ID2D1DeviceContext> m_context;
	wil::com_ptr<ID2D1Bitmap> bitmap;
	wil::com_ptr<IDXGISwapChain1> swapChain;
	wil::com_ptr<IDXGIDevice> dxgiDevice;
	wil::com_ptr<ID2D1Device> d2dDevice;
	wil::com_ptr<ID2D1Bitmap1> surfaceBitmap;
	wil::com_ptr<IDCompositionDevice> compositionDevice;
	wil::com_ptr<IDCompositionTarget> compositionTarget;
	wil::com_ptr<IDCompositionVisual> visual;
	wil::com_ptr<ID2D1RoundedRectangleGeometry> clipGeometry;

	float m_translation{};
	wil::com_ptr<ID2D1SolidColorBrush> progressBarFillBrush;

	D3D11Device d3d11Device;
	DXGIFactory dxgiFactory;

	constexpr static auto className = L"SplashWindowClass";
	static void registerClassIfNeeded()
	{
		static bool registered = false;
		
		if (registered)
			return;

		wicImagingFactory.emplace();

		WNDCLASS const windowClass
		{
			//.style = CS_HREDRAW | CS_VREDRAW,
			.lpfnWndProc = windowProc,
			.cbWndExtra = sizeof(SplashWindow*),
			/*.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH),*/
			.lpszClassName = className
		};
		RegisterClass(&windowClass);
		registered = true;
	}

	static auto getSelf(HWND hwnd)
	{
		return reinterpret_cast<SplashWindow*>(GetWindowLongPtr(hwnd, 0));
	}

	static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
			case WM_NCCREATE:
			{
				auto createStruct = reinterpret_cast<CREATESTRUCT*>(lparam);
				auto splashWindow = static_cast<SplashWindow*>(createStruct->lpCreateParams);
				SetWindowLongPtr(hwnd, 0, reinterpret_cast<LONG_PTR>(splashWindow));
				break;
			}
			case WM_TIMER:
			{
				switch (wparam)
				{
					case SplashWindow::MessageQueuePollingTimerId:
					{
						HWND mainAppHwnd;
						size_t receivedSize{};
						unsigned priority{};
						if (messageQueue->try_receive(&mainAppHwnd, sizeof(mainAppHwnd), receivedSize, priority))
						{
							KillTimer(hwnd, SplashWindow::MessageQueuePollingTimerId);
							syncMoveWithWindow = mainAppHwnd;
						}
						return 0;
					}
					case SplashWindow::AnimationTimerId:
					{
						auto self = getSelf(hwnd);
						self->m_translation = self->m_translation >= Config::ProgressBarWidth ? 0.0f : self->m_translation + 1.0f;
						InvalidateRect(hwnd, nullptr, true);
						return 0;
					}
					default:
						return 0;
				}
			}
			case WM_WINDOWPOSCHANGING:
			{
				if (syncMoveWithWindow)
				{
					auto windowPos = reinterpret_cast<WINDOWPOS*>(lparam);
					if (!(windowPos->flags & SWP_NOMOVE))
					{
						SetWindowPos(
							syncMoveWithWindow,
							nullptr,
							windowPos->x,
							windowPos->y,
							0,
							0,
							SWP_NOSIZE | SWP_NOACTIVATE
						);
					}
				}
				break;
			}
			//case WM_MOVE:
			//{
			//	if (syncMoveWithWindow)
			//	{
			//		auto xPos = (int)(short)LOWORD(lparam);   // horizontal position of client area
			//		auto yPos = (int)(short)HIWORD(lparam);   // vertical position of client area
			//		SetWindowPos(
			//			syncMoveWithWindow,
			//			nullptr,
			//			xPos,
			//			yPos,
			//			0,
			//			0,
			//			SWP_NOSIZE | SWP_NOACTIVATE
			//		);
			//	}
			//	break;
			//}
			case WM_ACTIVATE:
			{
				static bool firstTime = true;
				if (firstTime)
				{
					firstTime = false;
					StartupTimer::GetInstance().SetWindowShown();
					StartupTimer::GetInstance().Print(argv);
				}
				break;
			}
			case WM_PAINT:
			{
				auto self = getSelf(hwnd);
				//self->m_renderTarget->BeginDraw();
				//self->m_renderTarget->Clear(D2D1::ColorF(0x212121, 0.5f));
				//self->m_renderTarget->DrawBitmap(self->bitmap.get());
				//THROW_IF_FAILED(self->m_renderTarget->EndDraw());

				RECT rect;
				GetWindowRect(hwnd, &rect);

				D2D1_RECT_F bitmapDestination
				{
					.left = (rect.right - rect.left - Config::LogoWidth) / 2.f,
					.top = (rect.bottom - rect.top - Config::LogoHeight) / 2.f
				};
				bitmapDestination.right = bitmapDestination.left + Config::LogoWidth;
				bitmapDestination.bottom = bitmapDestination.top + Config::LogoHeight;


				self->m_context->BeginDraw();
				self->m_context->Clear(D2D1::ColorF(0x212121, 0.5f));
				self->m_context->DrawBitmap(self->bitmap.get(), &bitmapDestination);



				{
					wil::com_ptr<ID2D1Layer> layer;
					THROW_IF_FAILED(self->m_context->CreateLayer(layer.put()));

					self->m_context->PushLayer(
						D2D1::LayerParameters(D2D1::InfiniteRect(), self->clipGeometry.get()), 
						layer.get()
					);

					self->m_context->SetTransform(D2D1::Matrix3x2F::Translation(self->m_translation, 0.f));
					//draw progress bar
					auto left = (rect.right - rect.left - Config::ProgressBarWidth) / 2.f;
					auto top = rect.bottom - rect.top - Config::ProgressBarMarginBottom;
					self->m_context->FillRoundedRectangle(
						D2D1::RoundedRect(D2D1::RectF(left, top, left + Config::ProgressBarWidth / 2.0, top + Config::ProgressBarHeight),
							Config::ProgressBarRadius,
							Config::ProgressBarRadius
						),
						self->progressBarFillBrush.get()
					);

					self->m_context->PopLayer();
				}

				THROW_IF_FAILED(self->m_context->EndDraw());
				THROW_IF_FAILED(self->swapChain->Present(1, 0));
				THROW_IF_FAILED(self->compositionDevice->Commit());
				self->m_context->SetTransform(D2D1::Matrix3x2F::Identity());
				ValidateRect(hwnd, nullptr);
				return 0;
			}
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
			default:
				break;
		}
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	auto getInitialWindowSize()
	{
		//TODO：We'd better remember the window size to mimic UWP behavior
		return D2D1::SizeU(800, 600);
	}

	auto getLogoFilePath()
	{
		return LR"(C:\Users\Peter\Desktop\SplashScreenDemo\SplashScreenDemo (Package)\Images\Square150x150Logo.scale-200.png)";
	}

	void createWindow()
	{
		auto const [width, height] = getInitialWindowSize();
		m_hwnd = CreateWindowEx(
			WS_EX_NOREDIRECTIONBITMAP,
			className,
			L"SplashWindow",
			WS_OVERLAPPEDWINDOW,
			0,
			0,
			width,
			height,
			nullptr,
			nullptr,
			nullptr,
			this
		);


		messageQueue.emplace(m_hwnd);
		//THROW_IF_FAILED(d2d1Factory->CreateHwndRenderTarget(
		//	D2D1::RenderTargetProperties(
		//		D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_DEFAULT, 
		//		D2D1::PixelFormat(DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_PREMULTIPLIED)),
		//	D2D1::HwndRenderTargetProperties(m_hwnd, D2D1::SizeU(width, height)),
		//	m_renderTarget.put()
		//));

		//Create d2d
		{

			THROW_IF_FAILED(d3d11Device->QueryInterface(dxgiDevice.put()));
			THROW_IF_FAILED(DCompositionCreateDevice(dxgiDevice.get(), IID_PPV_ARGS(compositionDevice.put())));
			THROW_IF_FAILED(compositionDevice->CreateTargetForHwnd(m_hwnd, true, compositionTarget.put()));
			THROW_IF_FAILED(compositionDevice->CreateVisual(visual.put()));

			THROW_IF_FAILED(d2d1Factory->CreateDevice(dxgiDevice.get(), d2dDevice.put()));
			THROW_IF_FAILED(d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS::D2D1_DEVICE_CONTEXT_OPTIONS_NONE, m_context.put()));
			THROW_IF_FAILED(m_context->CreateSolidColorBrush(Config::ProgressBarFillColor, progressBarFillBrush.put()));
			swapChain = dxgiFactory.CreateSwapChainForComposition(width, height, dxgiDevice.get());

			THROW_IF_FAILED(visual->SetContent(swapChain.get()));
			THROW_IF_FAILED(compositionTarget->SetRoot(visual.get()));

			//get swap chain back buffer
			wil::com_ptr<IDXGISurface2> surface;
			swapChain->GetBuffer(0, IID_PPV_ARGS(surface.put()));
			D2D1_BITMAP_PROPERTIES1 bitmapProperty{};
			bitmapProperty.pixelFormat.alphaMode = D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_PREMULTIPLIED;
			bitmapProperty.pixelFormat.format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
			bitmapProperty.bitmapOptions = D2D1_BITMAP_OPTIONS::D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS::D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

			THROW_IF_FAILED(m_context->CreateBitmapFromDxgiSurface(surface.get(), &bitmapProperty, surfaceBitmap.put()));
			m_context->SetTarget(surfaceBitmap.get());
			//m_context->BeginDraw();
			//m_context->Clear(D2D1::ColorF(0x212121, 0.5f));

			auto left = (width - Config::ProgressBarWidth) / 2.f;
			auto top = height - Config::ProgressBarMarginBottom;
			THROW_IF_FAILED(d2d1Factory->CreateRoundedRectangleGeometry(
				D2D1::RoundedRect(D2D1::RectF(left, top, left + Config::ProgressBarWidth, top + Config::ProgressBarHeight),
					Config::ProgressBarRadius,
					Config::ProgressBarRadius
				),
				clipGeometry.put()
			));
		}

		//get the logo image, store in `bitmap`
		{
			wil::com_ptr<IWICBitmapDecoder> decoder;
			wil::com_ptr<IWICBitmapFrameDecode> frame;
			THROW_IF_FAILED((*wicImagingFactory)->CreateDecoderFromFilename(
				getLogoFilePath(),
				nullptr,
				GENERIC_READ,
				WICDecodeOptions::WICDecodeMetadataCacheOnDemand,
				decoder.put()));
			THROW_IF_FAILED(decoder->GetFrame(0, frame.put()));
			//convert format to 32bppPBGRA
			wil::com_ptr<IWICFormatConverter> converter;
			THROW_IF_FAILED((*wicImagingFactory)->CreateFormatConverter(converter.put()));
			THROW_IF_FAILED(converter->Initialize(
				frame.get(),
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherType::WICBitmapDitherTypeNone,
				nullptr,
				0.f,
				WICBitmapPaletteType::WICBitmapPaletteTypeCustom));

			THROW_IF_FAILED(m_context->CreateBitmapFromWicBitmap(
				converter.get(),
				bitmap.put()
			));
		}
		ShowWindow(m_hwnd, SW_SHOW);
		SetTimer(m_hwnd, MessageQueuePollingTimerId, MessageQueuePollingTimerIntervalMilliseconds, nullptr);
		SetTimer(m_hwnd, AnimationTimerId, AnimationTimerInterval, nullptr);
	}
public:
	constexpr static auto MessageQueuePollingTimerId = 0x1;
	constexpr static auto AnimationTimerId = 0x2;
	constexpr static auto MessageQueuePollingTimerIntervalMilliseconds = 100;
	constexpr static auto AnimationTimerInterval = 5;

	SplashWindow()
	{
		StartupTimer::GetInstance().SetBeforeMainWindowConstructed();
		registerClassIfNeeded();

		createWindow();
	}
};



int WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
)
{
	THROW_IF_FAILED(CoInitialize(nullptr));
	StartupTimer::GetInstance();
	argv = lpCmdLine;

	SplashWindow window;

	MSG msg;
	BOOL ret;
	while ((ret = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (ret == -1)
		{
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}
