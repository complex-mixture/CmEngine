#pragma once
#include <d3d12.h>
#include "Util.h"
#include <stdint.h>
#include "d3dx12.h"

struct FDescriptorHandle
{
	D3D12_CPU_DESCRIPTOR_HANDLE mCpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE mGpuHandle;
	void Offset(uint64_t _offset)
	{
		mCpuHandle.ptr += _offset;
		mGpuHandle.ptr += _offset;
	}
};

class FDescriptorHandleManagerBase : FNoncopyable
{
protected:
	FDescriptorHandleManagerBase() = default;
	uint32_t FindFreeSlot();
	void MarkUsedSlot(uint32_t _index);

public:
	void Init(D3D12_DESCRIPTOR_HEAP_TYPE _heapType, uint32_t _descriptorCount);
	void Clear();
	ID3D12DescriptorHeap * GetDescriptorHeap() { return mDescriptorHeap; }
	void FreeSlot(FDescriptorHandle _handle);
	void FreeSlot(D3D12_CPU_DESCRIPTOR_HANDLE _handle);
	void FreeSlot(D3D12_GPU_DESCRIPTOR_HANDLE _handle);
	void FreeSlot(uint32_t _index);

protected:
	uint32_t mDescriptorHandleSize;
	uint32_t mDescriptorCount;
	uint32_t * mBitMap = nullptr;
	uint32_t mBitMapIntCount = 0;
	FDescriptorHandle mDescriptorHandle;
	ID3D12DescriptorHeap * mDescriptorHeap;
};
