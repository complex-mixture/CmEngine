#pragma once
#include "ModuleManager.h"
#include "RenderSetting.h"
#include <d3d12.h>
#include <functional>

class FRenderScene;
class FD3D12Viewport;
class FCanvas;
class UWorld;

__interface IRenderModuleInterface : IModuleInterface
{
	//module interface
	void Init();
	void Clear();
	void RequestExit();

	//called on game thread
	void DeferCollectRenderFrameResource(FCanvas _canvas, UWorld * _world, FRenderSetting _renderSetting);
	void AddRenderThreadTask(std::function<void()> _function);
	void AddTaskOnFrameEnd(std::function<void()> _function);
	void AddTaskOnRenderThreadFlush(std::function<void()> _function);

	//called on game thread, keep the CollectRenderFrameResource called between the BeginFrame_GameThread and EndFrame_GameThead
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
