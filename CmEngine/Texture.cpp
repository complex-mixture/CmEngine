#include "Texture.h"
#include "DDSTextureLoader.h"
#include "D3D12RhiGlobal.h"
#include <wrl.h>
#include "D3D12RhiUtil.h"

void UTexture::Init(std::wstring _fileName)
{
	Microsoft::WRL::ComPtr<ID3D12Resource> textureGpu;
	Microsoft::WRL::ComPtr<ID3D12Resource> textureUploader;

	VerifyD3D12Result(DirectX::CreateDDSTextureFromFile12(
		GDevice,
		GCommandList,
		_fileName.c_str(),
		textureGpu,
		textureUploader,
		mData,
		mSubResourceData,
		&mNumSubresource
	)
	);

	textureGpu.CopyTo(&mTextureGpu);
	textureUploader.CopyTo(&mTextureUploadBuffer);

	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Texture2D.ResourceMinLODClamp = 0.0f;
	desc.Format = mTextureGpu->GetDesc().Format;
	desc.Texture2D.MipLevels = mTextureGpu->GetDesc().MipLevels;

	mDescriptorHandle = FDescriptorHandleManager<D3D12_SHADER_RESOURCE_VIEW_DESC>::Get().CreateView(mTextureGpu, desc);
}

void UTexture::Commit()
{
	GCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mTextureGpu,
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));

	// Use Heap-allocating UpdateSubresources implementation for variable number of subresources (which is the case for textures).
	UpdateSubresources(GCommandList, mTextureGpu, mTextureUploadBuffer, 0, 0, mNumSubresource, mSubResourceData.get());

	GCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mTextureGpu,
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

	mData = nullptr;
	mSubResourceData = nullptr;
}

void UTexture::EndCommit()
{
	mTextureUploadBuffer->Release();
	mTextureUploadBuffer = nullptr;
}

void UTexture::Clear()
{
	mTextureGpu->Release();
}
