#pragma once
#include "RenderModule.h"
#include "ModuleRegister.h"
#include <d3d12.h>
#include <vector>
#include <dxgi1_6.h>
#include "D3D12Viewport.h"
#include <stdint.h>
#include <mutex>
#include "CpuRenderFrameResource.h"
#include "GpuRenderFrameResource.h"

class FCanvas;

class FRenderModule : public IRenderModuleInterface
{
private:
	__forceinline FCpuRenderFrameResource * RequestCpuFrameRenderResource();
	__forceinline FGpuRenderFrameResource * RequestGpuFrameRenderResource();

	void RenderThreadMain();
public:
	//module interface
	void Init();
	void Clear();
	void RequestExit();

	//called on game thread
	void DeferCollectRenderFrameResource(FCanvas _canvas, UWorld * _world, FRenderSetting _renderSetting);
	void AddRenderThreadTask(std::function<void()> _function);
	void AddTaskOnFrameEnd(std::function<void()> _function);
	void AddTaskOnRenderThreadFlush(std::function<void()> _function);

	//
	void BeginFrame_GameThread();
	void CollectRenderFrameResource();
	void EndFrame_GameThead();

	//called on render thread
	void BeginFrame_RenderThread();
	void RenderScene_RenderThread();
	void EndFrame_RenderThead();
	bool ShouldExit();

private:
	std::thread mRenderThread;

	uint8_t mCurrentCpuFrameRenderResourceIndex = 0;
	std::array<FCpuRenderFrameResource, 3> mCpuFrameRenderResource;

	uint8_t mCurrentGpuFrameRenderResourceIndex = 0;
	std::array<FGpuRenderFrameResource, 3> mGpuFrameRenderResource;
};

RegistDefaultModule(FRenderModule, Render);
