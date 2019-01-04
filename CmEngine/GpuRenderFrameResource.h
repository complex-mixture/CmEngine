#pragma once
#include "Util.h"
#include <d3d12.h>
#include "D3D12Viewport.h"
#include "CpuRenderFrameResource.h"
#include "Canvas.h"
#include <set>
#include <functional>
#include "UploadBuffer.h"
#include "TreatedRenderInformation.h"
#include "PreTreatedRenderInformation.h"

struct FPreTreatedRenderInformation;
struct FTreatedRenderInformation;
class FCpuRenderFrameResource;

class FGpuRenderFrameResource : FNoncopyable
{
	friend FPreTreatedRenderInformation;
	friend FTreatedRenderInformation;
protected:
	void PreConstruct();
	void PostConstruct();

public:
	void Init(FCpuRenderFrameResource * _cpuRenderFrameResource, ID3D12CommandAllocator * _allocator);
	void Clear();
	void Reset();

	FCpuRenderFrameResource * GetCpuRenderFrameResource() { return mCpuRenderFrameResource; }
	std::vector<std::pair<FTreatedRenderInformation, FRenderSetting>> & GetTreatedRenderInformation() { return mTreatedRenderInformation; }

	void AddTaskOnRenderThreadFlush(std::function<void()> _function);

	void Construct();

	void BeginUse_RenderThread();
	void EndUse_RenderThread(uint64_t _fence);

	__forceinline std::set<FD3D12Viewport*> & GetPresentViewports() { return mPresentViewports; }

private:
	std::vector<std::function<void()>> mRenderThreadTasks;
	std::vector<std::function<void()>> mTasksOnFrameEnd;
	std::vector<std::function<void()>> mTasksOnRenderThreadFlush;
	std::vector<std::pair<FPreTreatedRenderInformation, FRenderSetting>> mPreTreatedRenderInformation;
	std::vector<std::pair<FTreatedRenderInformation, FRenderSetting>> mTreatedRenderInformation;
	FUploadConstantBuffer<PassCb> * mMainPassCb;
	FUploadConstantBuffer<ObjectCb> * mObjCb;

private:
	uint64_t mLastFence = 0;
	FCpuRenderFrameResource * mCpuRenderFrameResource;
	ID3D12CommandAllocator * mAllocator;
	std::set<FD3D12Viewport*> mPresentViewports;
#ifdef DEBUG
	bool mIsReady = false;
#endif // DEBUG
};
