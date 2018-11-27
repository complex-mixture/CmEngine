#include "CpuRenderFrameResource.h"

void FCpuRenderFrameResource::Init()
{
	mDataReadyEvent = CreateEvent(NULL, false, false, NULL);
	mDataEndUseEvent = CreateEvent(NULL, false, true, NULL);
}

void FCpuRenderFrameResource::Reset()
{
	mViewports.resize(0);
	mRenderInformations.resize(0);
}

void FCpuRenderFrameResource::DeferCollectRenderFrameResource(FD3D12Viewport * _renderViewport, FRenderSetting _renderSetting, FRenderScene * _renderScene)
{
	mViewports.push_back(_renderViewport);
	DeferCollectRenderFrameResource(_renderViewport->GetCurrentBackBuffer(), _renderSetting, _renderScene);
}

void FCpuRenderFrameResource::DeferCollectRenderFrameResource(ID3D12Resource * _renderTarget, FRenderSetting _renderSetting, FRenderScene * _renderScene)
{
	mRenderInformations.push_back(RenderInformation{ _renderTarget, _renderSetting });
}

void FCpuRenderFrameResource::Construct()
{
	Sleep(100);
}

void FCpuRenderFrameResource::BeginUse_GameThread()
{
	WaitForSingleObject(mDataEndUseEvent, INFINITE);
}

void FCpuRenderFrameResource::EndUse_GameThread()
{
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
