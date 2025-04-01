#include "ProgressBar.h"
#include <d2d1.h>
#include "Config.h"
#include <wil/result.h>
#include <wil/com.h>
#include "SplineDoubleKeyFrame.h"

ProgressBar::ProgressBar(ID2D1RenderTarget* renderTarget)
{
	THROW_IF_FAILED(renderTarget->CreateSolidColorBrush(Config::ProgressBarFillColor, progressBarFillBrush.put()));

}

void ProgressBar::OnPaint(ID2D1RenderTarget* renderTarget)
{
	wil::com_ptr<ID2D1Layer> layer;
	THROW_IF_FAILED(renderTarget->CreateLayer(layer.put()));
	renderTarget->PushLayer(
		D2D1::LayerParameters(D2D1::InfiniteRect(), clipGeometry.get()),
		layer.get()
	);
	renderTarget->SetTransform(D2D1::Matrix3x2F::Translation(m_translation, 0.f));

	//draw progress bar
	auto left = (m_width - Config::ProgressBarWidth) / 2.f;
	auto top = m_height - Config::ProgressBarMarginBottom;
	renderTarget->FillRoundedRectangle(
		D2D1::RoundedRect(D2D1::RectF(left, top, left + Config::ProgressBarWidth / 2.0, top + Config::ProgressBarHeight),
			Config::ProgressBarRadius,
			Config::ProgressBarRadius
		),
		progressBarFillBrush.get()
	);

	renderTarget->PopLayer();
}

void ProgressBar::OnSize(UINT width, UINT height, ID2D1Factory* d2d1Factory)
{
	ComponentBase::OnSize(width, height);

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

void ProgressBar::OnTick()
{
	m_translation = m_translation >= Config::ProgressBarWidth ? 0.0f : m_translation + 1.0f;
}

