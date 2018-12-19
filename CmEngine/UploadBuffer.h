#pragma once
#include "D3D12RhiUtil.h"
#include "D3D12RhiGlobal.h"
#include "d3dx12.h"

template<typename _constantBufferType>
class FUploadConstantBuffer
{
public:
	using ConstantBufferType = _constantBufferType;

	static constexpr uint64_t ElementSizeCpu = sizeof(ConstantBufferType);
	static constexpr uint64_t ElementSizeGpu = (ElementSizeCpu + 255) & ~255;

	FUploadConstantBuffer(uint64_t _elementCount)
	{
		mElementCount = _elementCount;

		VerifyD3D12Result(GDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(ElementSizeGpu * mElementCount),
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&mConstantBuffer)
		));

		VerifyD3D12Result(GDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(ElementSizeGpu * mElementCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mUploadBuffer)
		));
	}

	D3D12_GPU_VIRTUAL_ADDRESS GetElementGpuAddress(uint64_t _index)
	{
		D3D12_GPU_VIRTUAL_ADDRESS returned = mConstantBuffer->GetGPUVirtualAddress();
		returned += _index * ElementSizeGpu;
		return returned;
	}

	void BeginUpload()
	{
		mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mData));
	}

	void StoreElement(uint64_t _index, const ConstantBufferType & _value)
	{
		memcpy(mData + _index * ElementSizeGpu, &_value, ElementSizeCpu);
	}

	void Commit()
	{
		mUploadBuffer->Unmap(0, nullptr);
		mData = nullptr;
		GCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mConstantBuffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
		GCommandList->CopyResource(mConstantBuffer, mUploadBuffer);
		GCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mConstantBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COMMON));
	}

	~FUploadConstantBuffer()
	{
		mConstantBuffer->Release();
		mUploadBuffer->Release();
	}

private:
	uint64_t mElementCount;
	uint8_t * mData = nullptr;
	ID3D12Resource * mConstantBuffer = nullptr;
	ID3D12Resource * mUploadBuffer = nullptr;
};
