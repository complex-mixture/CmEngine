#include "Texture.h"
#include "Util.h"
#include <DirectXTex.h>
#include <fstream>
#pragma comment(lib, "DirectXTex.lib")

void FTexture::Construct(std::wstring _ddsFileName)
{
	std::ifstream ifs(_ddsFileName, std::ios::in | std::ios::binary);
	ifs.seekg(0, std::ios_base::end);
	mBufferLength = ifs.tellg();
	ifs.seekg(0, std::ios_base::beg);

	mBuffer = malloc(mBufferLength);
	ifs.read(reinterpret_cast<char*>(mBuffer), mBufferLength);
	ifs.close();

	DirectX::TexMetadata metaData;
	DWORD convFlag = 0;
	DirectX::GetMetadataFromDDSMemory(mBuffer, mBufferLength, 0, metaData, convFlag);
	mIsWithDx10Header = convFlag & 0x10000;//CONV_FLAGS_DX10, in DirectXTex\DirectXTexDDS.cpp, line 42

	mWidth = metaData.width;
	mHeight = metaData.height;
	mDepth = metaData.depth;
	mMipLevels = metaData.mipLevels;
	mArraySize = metaData.arraySize;
	mFormat = metaData.format;
	mDataLength = mBufferLength - (4 + 124 + (mIsWithDx10Header ? 20 : 0));//4 : magic Number, 124 : dds header, 20 : dds header dx10 

	switch (metaData.dimension)
	{
	case DirectX::TEX_DIMENSION_TEXTURE1D:
		mTexDimension = metaData.arraySize == 1 ? TexDimension::TEX_DIMENSION_1D : TexDimension::TEX_DIMENSION_1D_ARRAY;
		break;
	case DirectX::TEX_DIMENSION_TEXTURE2D:
		if (metaData.IsCubemap())
			mTexDimension = metaData.arraySize == 6 ? TexDimension::TEX_DIMENSION_CUBE : TexDimension::TEX_DIMENSION_CUBE_ARRAY;
		else
			mTexDimension = metaData.arraySize == 1 ? TexDimension::TEX_DIMENSION_2D : TexDimension::TEX_DIMENSION_2D_ARRAY;
		break;
	case DirectX::TEX_DIMENSION_TEXTURE3D:
		mTexDimension = TexDimension::TEX_DIMENSION_3D;
		break;
	default:
		Assert(false);
	}
}

void FTexture::SaveToFile(std::wstring _destFileName)
{
	std::ofstream ofs(_destFileName, std::ios::out | std::ios::binary);

	ofs.write(reinterpret_cast<const char*>(&MagicNumber), sizeof(uint64_t));

	ofs.write(reinterpret_cast<const char*>(&mWidth), sizeof(uint64_t));
	ofs.write(reinterpret_cast<const char*>(&mHeight), sizeof(uint64_t));
	ofs.write(reinterpret_cast<const char*>(&mDepth), sizeof(uint64_t));
	ofs.write(reinterpret_cast<const char*>(&mMipLevels), sizeof(uint64_t));
	ofs.write(reinterpret_cast<const char*>(&mArraySize), sizeof(uint64_t));
	ofs.write(reinterpret_cast<const char*>(&mFormat), sizeof(DXGI_FORMAT));
	ofs.write(reinterpret_cast<const char*>(&mTexDimension), sizeof(TexDimension));
	ofs.write(reinterpret_cast<const char*>(&mDataLength), sizeof(uint64_t));

	uint64_t offset = 4 + 124 + (mIsWithDx10Header ? 20 : 0);
	ofs.write(reinterpret_cast<const char*>((uint8_t *)mBuffer + offset), mDataLength);

	ofs.close();
}
