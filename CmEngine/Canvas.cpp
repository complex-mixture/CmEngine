#include "Canvas.h"

FCanvas FCanvas::Make(FD3D12Viewport * _viewport)
{
	FCanvas returned;

	returned.mViewport = _viewport;

	returned.mD3D12Viewport.TopLeftX = 0;
	returned.mD3D12Viewport.TopLeftY = 0;
	returned.mD3D12Viewport.Width = static_cast<float>(_viewport->GetWeight());
	returned.mD3D12Viewport.Height = static_cast<float>(_viewport->GetHeight());
	returned.mD3D12Viewport.MinDepth = 0.f;
	returned.mD3D12Viewport.MaxDepth = 1.f;

	returned.mScissorRect.left = 0;
	returned.mScissorRect.top = 0;
	returned.mScissorRect.bottom = _viewport->GetHeight();
	returned.mScissorRect.right = _viewport->GetWeight();

	return returned;
}
