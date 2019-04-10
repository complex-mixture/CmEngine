#pragma once
#include <d3d12.h>
#include <string>
#include "DescriptorHandleManager.h"
#include <memory>
#include <stdint.h>

enum class TexDimension : uint8_t
{
	TEX_DIMENSION_1D = 0,
	TEX_DIMENSION_1D_ARRAY = 1,
	TEX_DIMENSION_2D = 2,
	TEX_DIMENSION_2D_ARRAY = 3,
	TEX_DIMENSION_CUBE = 4,
	TEX_DIMENSION_CUBE_ARRAY = 5,
	TEX_DIMENSION_3D = 6,
};

class UTexture
{
	static constexpr uint64_t MagicNumber = 0x2711394101;

public:
	void Init(std::wstring _fileName);
	void CommitResource();
	void EndCommit();
	void Clear();

	FDescriptorHandle GetDescriptorHandle()const { return mDescriptorHandle; }

private:
	uint64_t mWidth = 0;
	uint64_t mHeight = 0;
	uint64_t mDepth = 0;
	uint64_t mMipLevels = 0;
	uint64_t mArraySize = 0;
	DXGI_FORMAT mFormat = DXGI_FORMAT_UNKNOWN;
	TexDimension mTexDimension;

	uint64_t mDataLength = 0;
	void * mData = nullptr;

	FDescriptorHandle mDescriptorHandle;
	ID3D12Resource * mTextureGpu = nullptr;
	ID3D12Resource * mTextureUploader = nullptr;
};
