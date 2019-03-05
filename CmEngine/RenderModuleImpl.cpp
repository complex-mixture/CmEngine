#include "RenderModuleImpl.h"
#include "Global.h"
#include "EngineLoop.h"
#include "D3D12RhiModule.h"
#include "Canvas.h"
#include <DirectXColors.h>
#include "d3dx12.h"
#include "RendererInterface.h"
#include "RendererCreater.h"
#include "ResourcePool.h"

FCpuRenderFrameResource * FRenderModule::RequestCpuFrameRenderResource()
{
	Assert(IsInGameThread());
	return &mCpuFrameRenderResource[mCurrentCpuFrameRenderResourceIndex];
}

FGpuRenderFrameResource * FRenderModule::RequestGpuFrameRenderResource()
{
	Assert(IsInRenderThread());
	return &mGpuFrameRenderResource[mCurrentGpuFrameRenderResourceIndex];
}

void FRenderModule::RenderThreadMain()
{
	GRenderThreadId = std::this_thread::get_id();
	FResourcePool::Get().Init();
	while (!ShouldExit())
	{
		BeginFrame_RenderThread();
		RenderScene_RenderThread();
		EndFrame_RenderThead();
		FResourcePool::Get().Tick();
	}
	FResourcePool::Get().Clear();
}

void FRenderModule::Init()
{
	for (int i = 0; i != 3; ++i)
	{
		mCpuFrameRenderResource[i].Init();
		mGpuFrameRenderResource[i].Init(&mCpuFrameRenderResource[i], GAllocators[i]);
	}

	mRenderThread = std::thread(std::mem_fn(&FRenderModule::RenderThreadMain), this);
}

void FRenderModule::Clear()
{
	FRenderModule::RequestExit();
	if (mRenderThread.joinable())
		mRenderThread.join();
	for (int i = 0; i != 3; ++i)
	{
		mCpuFrameRenderResource[i].Clear();
		mGpuFrameRenderResource[i].Clear();
	}
}

void FRenderModule::RequestExit()
{
	HANDLE event = CreateEvent(nullptr, false, false, nullptr);

	BeginFrame_GameThread();
	EndFrame_GameThead();
	BeginFrame_GameThread();
	EndFrame_GameThead();

	BeginFrame_GameThread();
	RequestCpuFrameRenderResource()->AddTaskOnFrameEnd([&]() {GFence->SetEventOnCompletion(GFence->Signal(), event); });
	EndFrame_GameThead();

	BeginFrame_GameThread();
	RequestCpuFrameRenderResource()->RequestExit();
	EndFrame_GameThead();

	WaitForSingleObject(event, INFINITE);
}

void FRenderModule::DeferCollectRenderFrameResource(FCanvas _canvas, UWorld * _world, FRenderSetting _renderSetting)
{
	Assert(IsInGameThread());
	RequestCpuFrameRenderResource()->DeferCollectRenderFrameResource(_canvas, _world, _renderSetting);
}

void FRenderModule::AddRenderThreadTask(std::function<void()> _function)
{
	Assert(IsInGameThread());
	RequestCpuFrameRenderResource()->AddRenderThreadTask(_function);
}

void FRenderModule::AddTaskOnFrameEnd(std::function<void()> _function)
{
	Assert(IsInGameThread());
	RequestCpuFrameRenderResource()->AddTaskOnFrameEnd(_function);
}

void FRenderModule::AddTaskOnRenderThreadFlush(std::function<void()> _function)
{
	if (IsInGameThread())
		RequestCpuFrameRenderResource()->AddTaskOnRenderThreadFlush(_function);
	else if (IsInRenderThread())
		RequestGpuFrameRenderResource()->AddTaskOnRenderThreadFlush(_function);
	else
		Assert(false);
}

void FRenderModule::BeginFrame_GameThread()
{
	FCpuRenderFrameResource * currentCpuFrameRenderResource = RequestCpuFrameRenderResource();
	currentCpuFrameRenderResource->BeginUse_GameThread();
	currentCpuFrameRenderResource->Reset();
}

void FRenderModule::CollectRenderFrameResource()
{
	FCpuRenderFrameResource * currentCpuFrameRenderResource = RequestCpuFrameRenderResource();
	currentCpuFrameRenderResource->Construct();
}

void FRenderModule::EndFrame_GameThead()
{
	FCpuRenderFrameResource * currentCpuFrameRenderResource = RequestCpuFrameRenderResource();
	currentCpuFrameRenderResource->EndUse_GameThread();
	mCurrentCpuFrameRenderResourceIndex = (mCurrentCpuFrameRenderResourceIndex + 1) % 3;
}

void FRenderModule::BeginFrame_RenderThread()
{
	FGpuRenderFrameResource * currentGpuRenderFrameResource = RequestGpuFrameRenderResource();
	currentGpuRenderFrameResource->BeginUse_RenderThread();
	currentGpuRenderFrameResource->Reset();
	currentGpuRenderFrameResource->Construct();
}

void FRenderModule::RenderScene_RenderThread()
{
	FGpuRenderFrameResource * currentGpuRenderFrameResource = RequestGpuFrameRenderResource();
	auto & treatedRenderInformations = currentGpuRenderFrameResource->GetTreatedRenderInformation();
	for (auto & _ele : treatedRenderInformations)
	{
		IRendererInterface * renderer = FRendererCreater::Get().CreateRenderer(_ele.second);
		renderer->RenderScene(&_ele.first);
		renderer->Release();
	}
}

void FRenderModule::EndFrame_RenderThead()
{
	FGpuRenderFrameResource * currentGpuRenderFrameResource = RequestGpuFrameRenderResource();
	GCommandList->Close();
	ID3D12CommandList * commandLists[] = { GCommandList };
	GCommandQueue->ExecuteCommandLists(1, commandLists);
	std::set<FD3D12Viewport*>& presentViewports = currentGpuRenderFrameResource->GetPresentViewports();
	for (auto _ele : presentViewports)
		_ele->Present();
	uint64_t fenceValue = GFence->Signal();
	for (auto _ele : presentViewports)
		_ele->SetLastUseFenceValue(fenceValue);
	currentGpuRenderFrameResource->EndUse_RenderThread(fenceValue);
	mCurrentGpuFrameRenderResourceIndex = (mCurrentGpuFrameRenderResourceIndex + 1) % 3;
}

bool FRenderModule::ShouldExit()
{
	return RequestGpuFrameRenderResource()->GetCpuRenderFrameResource()->ShouldExit();
}
