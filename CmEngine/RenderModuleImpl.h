#pragma once
#include "RenderModule.h"
#include "ModuleRegister.h"
#include <d3d12.h>
#include <vector>
#include <dxgi1_6.h>
#include "RenderScene.h"
#include "D3D12Viewport.h"
#include <stdint.h>
#include <mutex>
#include "CpuRenderFrameResource.h"
#include "GpuRenderFrameResource.h"

class FRenderModule : public IRenderModuleInterface
{
private:
	__forceinline FCpuRenderFrameResource * RequestCpuFrameRenderResource();
	__forceinline FGpuRenderFrameResource * RequestGpuFrameRenderReosurce();

	void RenderThreadMain();
public:
	//module interface
	void Init();
	void Clear();

	//called on game thread
	void DeferCollectRenderFrameResource(FD3D12Viewport * _renderViewport, FRenderSetting _renderSetting, FRenderScene * _renderScene);
	void DeferCollectRenderFrameResource(ID3D12Resource * _renderTarget, FRenderSetting _renderSetting, FRenderScene * _renderScene);
	void BeginFrame_GameThread();
	void CollectRenderFrameResource();
	void EndFrame_GameThead();

	//called on render thread
	void BeginFrame_RenderThread();
	void RenderScene_RenderThread();
	void EndFrame_RenderThead();

private:
	std::thread mRenderThread;

	uint8_t mCurrentCpuFrameRenderResourceIndex = 0;
	std::array<FCpuRenderFrameResource, 3> mCpuFrameRenderResource;

	uint8_t mCurrentGpuFrameRenderResourceIndex = 0;
	std::array<FGpuRenderFrameResource, 3> mGpuFrameRenderResource;
};

RegistDefaultModule(FRenderModule, Render);
