#include "TreatedRenderInformation.h"
#include "PreTreatedRenderInformation.h"
#include "TextuerPool.h"
#include "DescriptorHandleManager.h"
#include "GpuRenderFrameResource.h"

FTreatedRenderInformation::FTreatedRenderInformation(const FPreTreatedRenderInformation & _ptri, FGpuRenderFrameResource * _gpuRenderFrameResource)
{
	mRenderTarget = _ptri.mRenderTarget;
	mRenderTargetView = _ptri.mRenderTargetView;
	mD3DViewport = _ptri.mD3DViewport;
	mScissorRect = _ptri.mScissorRect;

	D3D12_DEPTH_STENCIL_VIEW_DESC desc;
	desc.Flags = D3D12_DSV_FLAG_NONE;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = 0;
	mDepthStencilBuffer = FTexturePool::Get().AllocateTexture(CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D24_UNORM_S8_UINT, mD3DViewport.Width, mD3DViewport.Height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL), D3D12_RESOURCE_STATE_DEPTH_WRITE, &CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D24_UNORM_S8_UINT, 1.f, 0));
	mDepthStencilView = FDescriptorHandleManager<D3D12_DEPTH_STENCIL_VIEW_DESC>::Get().CreateView(mDepthStencilBuffer, desc).mCpuHandle;
	_gpuRenderFrameResource->AddTaskOnRenderThreadFlush(
		[_resource = mDepthStencilBuffer, _view = mDepthStencilView]() {
		FTexturePool::Get().ReleaseTexture(_resource);
		FDescriptorHandleManager<D3D12_DEPTH_STENCIL_VIEW_DESC>::Get().FreeSlot(_view);
	});

	mMainPassCb = _gpuRenderFrameResource->mMainPassCb;
	mObjCb = _gpuRenderFrameResource->mObjCb;

	mMainPassCb->BeginUpload();
	mObjCb->BeginUpload();

	mMainPassCb->StoreElement(0, _ptri.mPassCb);

	mTreatedRenderStaticMeshs.resize(_ptri.mPreUreatedRenderStaticMeshs.size());
	for (size_t i = 0; i != _ptri.mPreUreatedRenderStaticMeshs.size(); ++i)
	{
		new(&mTreatedRenderStaticMeshs[i]) FTreatedRenderStaticMesh(_ptri.mPreUreatedRenderStaticMeshs[i]);
		mObjCb->StoreElement(i, _ptri.mPreUreatedRenderStaticMeshs[i].mObjCb);
		mTreatedRenderStaticMeshs[i].mIndexInObjCb = i;
	}

	mMainPassCb->Commit();
	mObjCb->Commit();
}

FTreatedRenderStaticMesh::FTreatedRenderStaticMesh(const FPreTreatedRenderStaticMesh & _ptsm)
{
	mIndicesCount = _ptsm.mIndicesCount;
	mVertexBufferView = _ptsm.mVertexBufferView;
	mIndexBufferView = _ptsm.mIndexBufferView;
	mRootSignature = _ptsm.mRootSignature;
	mPipelineState = _ptsm.mPipelineState;
	mShaderParameters = _ptsm.mShaderParameters;
}
