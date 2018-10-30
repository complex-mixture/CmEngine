#pragma once
#include "D3D12RhiGlobal.h"
#include <stdint.h>
#include <wtypes.h>
#include "Util.h"

class FD3D12Fence : FNoncopyable
{
private:
	ID3D12Fence * mFence;
	uint64_t mCurrentFenceValue = 0;

public:
	FD3D12Fence() :FD3D12Fence(0) {}

	FD3D12Fence(uint64_t _InitFenceValue) :mCurrentFenceValue(_InitFenceValue)
	{
		GDevice->CreateFence(mCurrentFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
	}

	__forceinline uint64_t GetCompleteValue()const
	{
		return mFence->GetCompletedValue();
	}

	__forceinline uint64_t Signal()
	{
		GCommandQueue->Signal(mFence, ++mCurrentFenceValue); return mCurrentFenceValue;
	}

	__forceinline HRESULT SetEventOnCompletion(uint64_t _value, HANDLE _event)
	{
		return mFence->SetEventOnCompletion(_value, _event);
	}

	__forceinline bool IsCompletion(uint64_t _value)
	{
		return mFence->GetCompletedValue() >= _value;
	}

	inline void WaitForCompletion(uint64_t _value)
	{
		if (!IsCompletion(_value))
		{
			HANDLE _event = CreateEvent(nullptr, false, false, nullptr);
			SetEventOnCompletion(_value, _event);
			WaitForSingleObject(_event, INFINITE);
			CloseHandle(_event);
		}
	}

	~FD3D12Fence() { mFence->Release(); }
};