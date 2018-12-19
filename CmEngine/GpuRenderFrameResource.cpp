#include "GpuRenderFrameResource.h"
#include "D3D12RhiGlobal.h"
#include "D3D12Fence.h"
#include "CpuRenderFrameResource.h"
#include "D3D12RhiModule.h"
#include "PreTreatedRenderInformation.h"
#include "StaticMesh.h"
#include "TreatedRenderInformation.h"
#include "TextuerPool.h"
#include "DescriptorHandleManager.h"
#include "UnTreatedRenderInformation.h"
#include "Materials.h"

void FGpuRenderFrameResource::PreConstruct()
{
	Assert(mIsReady);
	mRenderThreadTasks = std::move(mCpuRenderFrameResource->mRenderThreadTasks);
	mTasksOnFrameEnd = std::move(mCpuRenderFrameResource->mTasksOnFrameEnd);
	mTasksOnRenderThreadFlush = std::move(mCpuRenderFrameResource->mTasksOnRenderThreadFlush);

	for (auto & _ele : mCpuRenderFrameResource->mCanvases)
	{
		if (_ele.mViewport != nullptr)
			mPresentViewports.insert(_ele.mViewport);
	}

	for (auto _ele : mRenderThreadTasks)
		_ele();

	mPreTreatedRenderInformation.reserve(mCpuRenderFrameResource->mUntreatedRenderInformations.size());

	for (auto _ele : mCpuRenderFrameResource->mUntreatedRenderInformations)
	{
		FPreTreatedRenderInformation ptri(_ele.first, this);
		mPreTreatedRenderInformation.push_back(std::make_pair(std::move(ptri), _ele.second));
	}
}

void FGpuRenderFrameResource::PostConstruct()
{
	for (auto & _ele : mPreTreatedRenderInformation)
	{
		FTreatedRenderInformation tri(_ele.first, this);
		mTreatedRenderInformation.push_back(std::make_pair(std::move(tri), _ele.second));
	}
}

void FGpuRenderFrameResource::Init(FCpuRenderFrameResource * _cpuRenderFrameResource, ID3D12CommandAllocator * _allocator)
{
	mCpuRenderFrameResource = _cpuRenderFrameResource;
	mAllocator = _allocator;
	mMainPassCb = new FUploadConstantBuffer<PassCb>(1);
	mObjCb = new FUploadConstantBuffer<ObjectCb>(64);
}

void FGpuRenderFrameResource::Clear()
{
	delete mMainPassCb;
	delete mObjCb;
}

void FGpuRenderFrameResource::Reset()
{
	for (auto _ele : mTasksOnRenderThreadFlush)
	{
		_ele();
	}
	mTasksOnRenderThreadFlush.resize(0);
	mRenderThreadTasks.resize(0);
	mPresentViewports.clear();
	mPreTreatedRenderInformation.resize(0);
	mTreatedRenderInformation.resize(0);
#ifdef DEBUG
	mIsReady = true;
#endif // DEBUG
}

void FGpuRenderFrameResource::AddTaskOnRenderThreadFlush(std::function<void()> _function)
{
	Assert(mIsReady);
	mTasksOnRenderThreadFlush.push_back(_function);
}

void FGpuRenderFrameResource::Construct()
{
	PreConstruct();
	mCpuRenderFrameResource->EndUse_RenderThread();
	PostConstruct();
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
#ifdef DEBUG
	mIsReady = false;
#endif // DEBUG
	mLastFence = _fence;
	for (auto _ele : mTasksOnFrameEnd)
		_ele();
}
