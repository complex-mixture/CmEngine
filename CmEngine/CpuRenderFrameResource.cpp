#include "CpuRenderFrameResource.h"
#include "Canvas.h"
#include "World.h"
#include "CameraActor.h"
#include "StaticMeshActor.h"
#include "DirectionLightActor.h"

void FCpuRenderFrameResource::Init()
{
	mDataReadyEvent = CreateEvent(NULL, false, false, NULL);
	mDataEndUseEvent = CreateEvent(NULL, false, true, NULL);
}

void FCpuRenderFrameResource::Clear()
{
	CloseHandle(mDataEndUseEvent);
	CloseHandle(mDataReadyEvent);
	mDataEndUseEvent = nullptr;
	mDataReadyEvent = nullptr;
}

void FCpuRenderFrameResource::Reset()
{
	mRenderThreadTasks.resize(0);
	mTasksOnFrameEnd.resize(0);
	mTasksOnRenderThreadFlush.resize(0);
	mCollectedRenderRequest.resize(0);
	mUntreatedRenderInformations.resize(0);
	mCanvases.resize(0);
	mCollectedRenderRequest.resize(0);
#ifdef DEBUG
	mIsReady = true;
#endif // DEBUG
}

void FCpuRenderFrameResource::DeferCollectRenderFrameResource(FCanvas _canvas, UWorld * _world, FRenderSetting _renderSetting)
{
	Assert(mIsReady);
	mCanvases.push_back(_canvas);
	mCollectedRenderRequest.push_back(std::make_tuple(_canvas, _world, _renderSetting));
}

void FCpuRenderFrameResource::AddRenderThreadTask(std::function<void()> _function)
{
	Assert(mIsReady);
	mRenderThreadTasks.push_back(_function);
}

void FCpuRenderFrameResource::AddTaskOnFrameEnd(std::function<void()> _function)
{
	Assert(mIsReady);
	mTasksOnFrameEnd.push_back(_function);
}

void FCpuRenderFrameResource::AddTaskOnRenderThreadFlush(std::function<void()> _function)
{
	Assert(mIsReady);
	mTasksOnRenderThreadFlush.push_back(_function);
}

void FCpuRenderFrameResource::Construct()
{
	Assert(mIsReady);
	for (auto & _ele : mCollectedRenderRequest)
	{
		FUntreatedRenderInformation utri(std::get<0>(_ele), std::get<1>(_ele), this);
		mUntreatedRenderInformations.push_back(std::make_pair(std::move(utri), std::get<2>(_ele)));
	}
}

void FCpuRenderFrameResource::BeginUse_GameThread()
{
	WaitForSingleObject(mDataEndUseEvent, INFINITE);
}

void FCpuRenderFrameResource::EndUse_GameThread()
{
#ifdef DEBUG
	mIsReady = false;
#endif // DEBUG
	SetEvent(mDataReadyEvent);
}

void FCpuRenderFrameResource::BeginUse_RenderThread()
{
	WaitForSingleObject(mDataReadyEvent, INFINITE);
}

void FCpuRenderFrameResource::EndUse_RenderThread()
{
	SetEvent(mDataEndUseEvent);
}
