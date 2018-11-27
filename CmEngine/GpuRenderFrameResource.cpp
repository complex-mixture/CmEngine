#include "GpuRenderFrameResource.h"
#include "D3D12RhiGlobal.h"
#include "D3D12Fence.h"
#include "CpuRenderFrameResource.h"

void FGpuRenderFrameResource::Init(FCpuRenderFrameResource * _cpuRenderFrameResource, ID3D12CommandAllocator * _allocator)
{
	mCpuRenderFrameResource = _cpuRenderFrameResource;
	mAllocator = _allocator;
}

void FGpuRenderFrameResource::Reset()
{
}

void FGpuRenderFrameResource::Construct()
{
	mPresentViewports = std::move(mCpuRenderFrameResource->mViewports);
	mRenderInformations = std::move(mCpuRenderFrameResource->mRenderInformations);
	Sleep(200);
	mCpuRenderFrameResource->EndUse_RenderThread();
}

void FGpuRenderFrameResource::BeginUse_RenderThread()
{
	GFence->WaitForCompletion(mLastFence);
	mCpuRenderFrameResource->BeginUse_RenderThread();
	mAllocator->Reset();
	GCommandList->Reset(mAllocator, nullptr);
}

void FGpuRenderFrameResource::EndUse_RenderThread(uint64_t _fence)
{
	mLastFence = _fence;
}
