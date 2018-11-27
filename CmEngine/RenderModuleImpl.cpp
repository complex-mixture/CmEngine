#include "RenderModuleImpl.h"
#include "Global.h"
#include "EngineLoop.h"
#include "D3D12RhiModule.h"

FCpuRenderFrameResource * FRenderModule::RequestCpuFrameRenderResource()
{
	return &mCpuFrameRenderResource[mCurrentCpuFrameRenderResourceIndex];
	return &mCpuFrameRenderResource[0];
}

FGpuRenderFrameResource * FRenderModule::RequestGpuFrameRenderReosurce()
{
	return &mGpuFrameRenderResource[mCurrentGpuFrameRenderResourceIndex];
	return &mGpuFrameRenderResource[0];
}

void FRenderModule::RenderThreadMain()
{
	GRenderThreadId == std::this_thread::get_id();
	while (!GEngineLoop.ShouldExit())
	{
		BeginFrame_RenderThread();
		RenderScene_RenderThread();
		EndFrame_RenderThead();
	}
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
	if (mRenderThread.joinable())
		mRenderThread.join();
}

void FRenderModule::DeferCollectRenderFrameResource(FD3D12Viewport * _renderViewport, FRenderSetting _renderSetting, FRenderScene * _renderScene)
{
	RequestCpuFrameRenderResource()->DeferCollectRenderFrameResource(_renderViewport, _renderSetting, _renderScene);
}

void FRenderModule::DeferCollectRenderFrameResource(ID3D12Resource * _renderTarget, FRenderSetting _renderSetting, FRenderScene * _renderScene)
{
	RequestCpuFrameRenderResource()->DeferCollectRenderFrameResource(_renderTarget, _renderSetting, _renderScene);
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
	FGpuRenderFrameResource * currentGpuRenderFrameResource = RequestGpuFrameRenderReosurce();
	currentGpuRenderFrameResource->BeginUse_RenderThread();
	currentGpuRenderFrameResource->Reset();
	currentGpuRenderFrameResource->Construct();
}

void FRenderModule::RenderScene_RenderThread()
{
	//TODO: 
}

void FRenderModule::EndFrame_RenderThead()
{
	FGpuRenderFrameResource * currentGpuRenderFrameResource = RequestGpuFrameRenderReosurce();
	GCommandList->Close();
	ID3D12CommandList * commandLists[] = { GCommandList };
	GCommandQueue->ExecuteCommandLists(1, commandLists);
	std::vector<FD3D12Viewport*>& presentViewports = currentGpuRenderFrameResource->GetPresentViewports();
	for (auto _ele : presentViewports)
		_ele->Present();
	uint64_t fenceValue = GFence->Signal();
	for (auto _ele : presentViewports)
		_ele->SetLastUseFenceValue(fenceValue);
	currentGpuRenderFrameResource->EndUse_RenderThread(fenceValue);
	mCurrentGpuFrameRenderResourceIndex = (mCurrentGpuFrameRenderResourceIndex + 1) % 3;

}
