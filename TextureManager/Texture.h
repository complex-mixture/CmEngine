#pragma once
#include <string>
#include <dxgiformat.h>

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

class FTexture
{
	static constexpr uint64_t MagicNumber = 0x2711394101;
public:
	FTexture() = default;
	void Construct(std::wstring _ddsFileName);
	void SaveToFile(std::wstring _destFileName);
	~FTexture() { if (mBuffer) free(mBuffer); }

private:
	uint64_t mWidth;
	uint64_t mHeight;
	uint64_t mDepth;
	uint64_t mMipLevels;
	uint64_t mArraySize;
	DXGI_FORMAT mFormat;
	TexDimension mTexDimension;
	uint64_t mDataLength;

	bool mIsWithDx10Header;
	void * mBuffer = nullptr;
	uint64_t mBufferLength;
};