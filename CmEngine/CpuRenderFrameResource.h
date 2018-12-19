#pragma once
#include "Util.h"
#include "RenderSetting.h"
#include <d3d12.h>
#include <vector>
#include "Canvas.h"
#include "UnTreatedRenderInformation.h"
#include <functional>

struct FUntreatedRenderInformation;
class UWorld;

class FCpuRenderFrameResource : FNoncopyable
{
	friend FUntreatedRenderInformation;
public:
	void Init();
	void Clear();
	void Reset();

	void RequestExit() { mShuldExit = true; }
	bool ShouldExit()const { return mShuldExit; }

	void DeferCollectRenderFrameResource(FCanvas _canvas, UWorld * _world, FRenderSetting _renderSetting);
	void AddRenderThreadTask(std::function<void()> _function);
	void AddTaskOnFrameEnd(std::function<void()> _function);
	void AddTaskOnRenderThreadFlush(std::function<void()> _function);

	void Construct();

	void BeginUse_GameThread();
	void EndUse_GameThread();
	void BeginUse_RenderThread();
	void EndUse_RenderThread();

public:
	std::vector<std::pair<FUntreatedRenderInformation, FRenderSetting>> mUntreatedRenderInformations;
	std::vector<FCanvas> mCanvases;
	std::vector<std::function<void()>> mRenderThreadTasks;
	std::vector<std::function<void()>> mTasksOnFrameEnd;
	std::vector<std::function<void()>> mTasksOnRenderThreadFlush;
	bool mShuldExit;

private:
	std::vector<std::tuple<FCanvas, UWorld*, FRenderSetting>> mCollectedRenderRequest;

private:
	HANDLE mDataReadyEvent = nullptr;
	HANDLE mDataEndUseEvent = nullptr;
#ifdef DEBUG
	bool mIsReady = false;
#endif // DEBUG
};
