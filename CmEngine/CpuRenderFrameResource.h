#pragma once
#include "Util.h"
#include <mutex>
#include "RenderSetting.h"
#include "RenderScene.h"
#include "D3D12Viewport.h"
#include <d3d12.h>
#include <map>

struct RenderInformation
{
	ID3D12Resource * mRenderTarget = nullptr;
	FRenderSetting mRenderSetting;
};

class FCpuRenderFrameResource : FNoncopyable
{
public:
	void Init();
	void Reset();

	void DeferCollectRenderFrameResource(FD3D12Viewport * _renderViewport, FRenderSetting _renderSetting, FRenderScene * _renderScene);
	void DeferCollectRenderFrameResource(ID3D12Resource * _renderTarget, FRenderSetting _renderSetting, FRenderScene * _renderScene);

	void Construct();

	void BeginUse_GameThread();
	void EndUse_GameThread();
	void BeginUse_RenderThread();
	void EndUse_RenderThread();

public:
	std::vector<FD3D12Viewport*> mViewports;
	std::vector<RenderInformation> mRenderInformations;

private:
	HANDLE mDataReadyEvent = nullptr;
	HANDLE mDataEndUseEvent = nullptr;
};