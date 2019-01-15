#include "Texture.h"
#include "D3D12RhiGlobal.h"
#include <fstream>
#include "D3D12RhiUtil.h"
#include "Common.h"
#include "TextureLoaderHelper.h"
#include <winerror.h>

void UTexture::Init(std::wstring _fileName)
{
	std::ifstream ifs(_fileName, std::ios::in | std::ios::binary);

	uint64_t magicNumber = 0;
	ifs.read(reinterpret_cast<char *>(&magicNumber), sizeof(uint64_t));
	Assert(magicNumber == MagicNumber);

	ifs.read(reinterpret_cast<char*>(&mWidth), sizeof(uint64_t));
	ifs.read(reinterpret_cast<char*>(&mHeight), sizeof(uint64_t));
	ifs.read(reinterpret_cast<char*>(&mDepth), sizeof(uint64_t));
	ifs.read(reinterpret_cast<char*>(&mMipLevels), sizeof(uint64_t));
	ifs.read(reinterpret_cast<char*>(&mArraySize), sizeof(uint64_t));
	ifs.read(reinterpret_cast<char*>(&mFormat), sizeof(DXGI_FORMAT));
	ifs.read(reinterpret_cast<char*>(&mTexDimension), sizeof(TexDimension));

	ifs.read(reinterpret_cast<char*>(&mDataLength), sizeof(uint64_t));
	Assert(mDataLength != 0);
	mData = malloc(mDataLength);
	ifs.read(reinterpret_cast<char*>(mData), mDataLength);
	ifs.close();

	CD3DX12_RESOURCE_DESC resourceDesc;

	switch (mTexDimension)
	{
	case TexDimension::TEX_DIMENSION_1D:
	case TexDimension::TEX_DIMENSION_1D_ARRAY:
		resourceDesc = CD3DX12_RESOURCE_DESC::Tex1D(mFormat, mWidth, mArraySize, mMipLevels);
		break;
	case TexDimension::TEX_DIMENSION_2D:
	case TexDimension::TEX_DIMENSION_2D_ARRAY:
	case TexDimension::TEX_DIMENSION_CUBE:
	case TexDimension::TEX_DIMENSION_CUBE_ARRAY:
		resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(mFormat, mWidth, mHeight, mArraySize, mMipLevels);
		break;
	case TexDimension::TEX_DIMENSION_3D:
		resourceDesc = CD3DX12_RESOURCE_DESC::Tex3D(mFormat, mWidth, mHeight, mDepth, mMipLevels);
		break;
	default:
		Assert(false);
	}

	VerifyD3D12Result(GDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&mTextureGpu)
	));

	VerifyD3D12Result(GDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(GetRequiredIntermediateSize(mTextureGpu, 0, resourceDesc.DepthOrArraySize * resourceDesc.MipLevels)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mTextureUploader)
	));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = mFormat;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	switch (mTexDimension)
	{
	case TexDimension::TEX_DIMENSION_1D:
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
		srvDesc.Texture1D.MostDetailedMip = 0;
		srvDesc.Texture1D.MipLevels = mMipLevels;
		srvDesc.Texture1D.ResourceMinLODClamp = 0.f;
		break;
	case TexDimension::TEX_DIMENSION_1D_ARRAY:
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
		srvDesc.Texture1DArray.MostDetailedMip = 0;
		srvDesc.Texture1DArray.MipLevels = mMipLevels;
		srvDesc.Texture1DArray.FirstArraySlice = 0;
		srvDesc.Texture1DArray.ArraySize = mArraySize;
		srvDesc.Texture1DArray.ResourceMinLODClamp = 0.f;
		break;
	case TexDimension::TEX_DIMENSION_2D:
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = mMipLevels;
		srvDesc.Texture2D.PlaneSlice = 0;
		srvDesc.Texture2D.ResourceMinLODClamp = 0.f;
		break;
	case TexDimension::TEX_DIMENSION_2D_ARRAY:
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
		srvDesc.Texture2DArray.MostDetailedMip = 0;
		srvDesc.Texture2DArray.MipLevels = mMipLevels;
		srvDesc.Texture2DArray.FirstArraySlice = 0;
		srvDesc.Texture2DArray.ArraySize = mArraySize;
		srvDesc.Texture2DArray.PlaneSlice = 0;
		srvDesc.Texture2DArray.ResourceMinLODClamp = 0.f;
		break;
	case TexDimension::TEX_DIMENSION_CUBE:
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.TextureCube.MipLevels = mMipLevels;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.f;
		break;
	case TexDimension::TEX_DIMENSION_CUBE_ARRAY:
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
		srvDesc.TextureCubeArray.MostDetailedMip = 0;
		srvDesc.TextureCubeArray.MipLevels = mMipLevels;
		srvDesc.TextureCubeArray.First2DArrayFace = 0;
		srvDesc.TextureCubeArray.NumCubes = mArraySize / 6;
		srvDesc.TextureCubeArray.ResourceMinLODClamp = 0.f;
		break;
	case TexDimension::TEX_DIMENSION_3D:
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
		srvDesc.Texture3D.MostDetailedMip = 0;
		srvDesc.Texture3D.MipLevels = mMipLevels;
		srvDesc.Texture3D.ResourceMinLODClamp = 0.f;
		break;
	default:
		Assert(false);
	}

	mDescriptorHandle = FDescriptorHandleManager<D3D12_SHADER_RESOURCE_VIEW_DESC>::Get().CreateView(mTextureGpu, srvDesc);
}

void UTexture::CommitResource()
{
	std::vector<D3D12_SUBRESOURCE_DATA> subresourceDatas;
	subresourceDatas.resize(mMipLevels * mArraySize);

	uint8_t * data = reinterpret_cast<uint8_t*>(mData);
	uint8_t * dataEnd = data + mDataLength;
	size_t numBytes = 0;
	size_t rowBytes = 0;
	size_t index = 0;
	for (uint64_t i = 0; i != mArraySize; ++i)
	{
		size_t w = mWidth;
		size_t h = mHeight;
		size_t d = mDepth;
		for (size_t j = 0; j < mMipLevels; j++)
		{
			HRESULT hr = GetSurfaceInfo(w, h, mFormat, &numBytes, &rowBytes, nullptr);
			Assert(hr == S_OK && numBytes <= UINT32_MAX && rowBytes <= UINT32_MAX);
			D3D12_SUBRESOURCE_DATA res =
			{
				reinterpret_cast<const void*>(data),
				static_cast<LONG_PTR>(rowBytes),
				static_cast<LONG_PTR>(numBytes)
			};

			AdjustPlaneResource(mFormat, h, 0, res);

			subresourceDatas[i * mMipLevels + j] = res;

			Assert(data + (numBytes * d) <= dataEnd);

			data += numBytes * d;

			w = w >> 1;
			h = h >> 1;
			d = d >> 1;

			w = w ? w : 1;
			h = h ? h : 1;
			d = d ? d : 1;
		}
	}

	UpdateSubresources(GCommandList, mTextureGpu, mTextureUploader, 0, 0, subresourceDatas.size(), &subresourceDatas[0]);

	GCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mTextureGpu,
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

	free(mData);
	mData = nullptr;
}

void UTexture::EndCommit()
{
	mTextureUploader->Release();
	mTextureUploader = nullptr;
}

void UTexture::Clear()
{
	mTextureGpu->Release();
	mTextureGpu = nullptr;
}
