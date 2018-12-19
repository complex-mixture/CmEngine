#include "DescriptorHandleManagerBase.h"
#include "D3D12RhiGlobal.h"
#include "D3D12RhiUtil.h"
#include <intsafe.h>

uint32_t FDescriptorHandleManagerBase::FindFreeSlot()
{
	uint32_t bitMapIntIndex;
	for (bitMapIntIndex = 0; bitMapIntIndex != mBitMapIntCount - 1; ++bitMapIntIndex)
	{
		if (bitMapIntIndex != UINT32_MAX)
			break;
	}
	uint32_t bitMapBitIndex;
	uint32_t mask;
	for (bitMapBitIndex = 0, mask = 1; bitMapBitIndex != 32; ++bitMapBitIndex, mask <<= 1)
	{
		if ((mBitMap[bitMapIntIndex] & mask) == 0)
			break;
	}
	if (bitMapIntIndex * 32 + bitMapBitIndex == mDescriptorCount)
		return UINT32_ERROR;
	else
		return bitMapIntIndex * 32 + bitMapBitIndex;
}

void FDescriptorHandleManagerBase::MarkUsedSlot(uint32_t _index)
{
	Assert(_index < mDescriptorCount);
	mBitMap[_index / 32] |= 1 << _index % 32;
}

void FDescriptorHandleManagerBase::Init(D3D12_DESCRIPTOR_HEAP_TYPE _heapType, uint32_t _descriptorCount)
{
	mDescriptorCount = _descriptorCount;
	mBitMapIntCount = (mDescriptorCount + 31) % 32;
	mBitMap = new uint32_t[mBitMapIntCount];
	memset(mBitMap, 0x00, mBitMapIntCount * 4);
	D3D12_DESCRIPTOR_HEAP_DESC desc;
	if (_heapType == D3D12_DESCRIPTOR_HEAP_TYPE_DSV || _heapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	else
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;
	desc.NumDescriptors = _descriptorCount;
	desc.Type = _heapType;
	VerifyD3D12Result(GDevice->CreateDescriptorHeap(
		&desc,
		IID_PPV_ARGS(&mDescriptorHeap)
	));
	mDescriptorHandle.mCpuHandle = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	mDescriptorHandle.mGpuHandle = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	mDescriptorHandleSize = GDevice->GetDescriptorHandleIncrementSize(_heapType);
}

void FDescriptorHandleManagerBase::Clear()
{
	delete[] mBitMap;
	mDescriptorHeap->Release();
	mDescriptorHeap = nullptr;
}

void FDescriptorHandleManagerBase::FreeSlot(FDescriptorHandle _handle)
{
	Assert(_handle.mCpuHandle.ptr - mDescriptorHandle.mCpuHandle.ptr == _handle.mGpuHandle.ptr - mDescriptorHandle.mGpuHandle.ptr && _handle.mCpuHandle.ptr >= mDescriptorHandle.mCpuHandle.ptr);
	FreeSlot((_handle.mCpuHandle.ptr - mDescriptorHandle.mCpuHandle.ptr) / mDescriptorHandleSize);
}

void FDescriptorHandleManagerBase::FreeSlot(D3D12_CPU_DESCRIPTOR_HANDLE _handle)
{
	Assert(_handle.ptr >= mDescriptorHandle.mCpuHandle.ptr);
	FreeSlot((_handle.ptr - mDescriptorHandle.mCpuHandle.ptr) / mDescriptorHandleSize);
}

void FDescriptorHandleManagerBase::FreeSlot(D3D12_GPU_DESCRIPTOR_HANDLE _handle)
{
	Assert(_handle.ptr >= mDescriptorHandle.mGpuHandle.ptr);
	FreeSlot((_handle.ptr - mDescriptorHandle.mGpuHandle.ptr) / mDescriptorHandleSize);
}

void FDescriptorHandleManagerBase::FreeSlot(uint32_t _index)
{
	Assert(_index < mDescriptorCount);
	mBitMap[_index / 32] &= ~(1 << _index % 32);
}
