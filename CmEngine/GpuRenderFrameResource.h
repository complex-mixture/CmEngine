#pragma once
#include "Util.h"
#include <d3d12.h>
#include "D3D12Viewport.h"
#include "CpuRenderFrameResource.h"
class	FCpuRenderFrameResource;
class FGpuRenderFrameResource : FNoncopyable
{
public:
	//called at begin
	void Init(FCpuRenderFrameResource * _cpuRenderFrameResource, ID3D12CommandAllocator * _allocator);
	//called after BeginUse_RenderThread
	void Reset();

	void Construct();

	void BeginUse_RenderThread();
	//should be called after commit the command to queue, and set the last fence, not after Process the cpu render frame resource
	void EndUse_RenderThread(uint64_t _fence);

	__forceinline std::vector<FD3D12Viewport*> & GetPresentViewports() { return mPresentViewports; }

public:
	std::vector<RenderInformation> mRenderInformations;

private:
	uint64_t mLastFence = 0;
	FCpuRenderFrameResource * mCpuRenderFrameResource;
	ID3D12CommandAllocator * mAllocator;
	std::vector<FD3D12Viewport*> mPresentViewports;
};