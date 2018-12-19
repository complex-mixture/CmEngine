#pragma once
#include <d3d12.h>
#include <string>
#include "DescriptorHandleManager.h"
#include <memory>
class UTexture
{
public:
	void Init(std::wstring _fileName);
	void Commit();
	void EndCommit();
	void Clear();

	FDescriptorHandle GetDescriptorHandle()const { return mDescriptorHandle; }

private:
	std::unique_ptr<uint8_t[]> mData;
	std::unique_ptr<D3D12_SUBRESOURCE_DATA[]> mSubResourceData;
	UINT mNumSubresource;

	FDescriptorHandle mDescriptorHandle;
	ID3D12Resource * mTextureGpu;
	ID3D12Resource * mTextureUploadBuffer;
};