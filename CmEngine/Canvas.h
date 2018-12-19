#pragma once
#include "D3D12Viewport.h"
#include <d3d12.h>
class FCanvas
{
public:
	FCanvas() {};
	static FCanvas Make(FD3D12Viewport * _viewport);

	float GetAspectRatio()const { return static_cast<float>(mD3D12Viewport.Width) / mD3D12Viewport.Height; }

public:
	//optional
	FD3D12Viewport * mViewport = nullptr;

	ID3D12Resource * mRendertarget = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE mRendertargetView;

	D3D12_VIEWPORT mD3D12Viewport;
	D3D12_RECT mScissorRect;
};
