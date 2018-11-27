#pragma once
#include "ModuleManager.h"
#include "RenderSetting.h"
#include <d3d12.h>
class FRenderScene;
class FD3D12Viewport;

__interface IRenderModuleInterface : IModuleInterface
{
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
};

inline IRenderModuleInterface * GetRenderModule()
{
	return FModuleManager::Get().GetModule<IRenderModuleInterface>(L"Render");
}
