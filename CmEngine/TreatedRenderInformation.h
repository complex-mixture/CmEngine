#pragma once
#include "UploadBuffer.h"
#include "ConstantBufferSruct.h"
#include <d3d12.h>
#include "ShaderParameter.h"

class FGpuRenderFrameResource;
struct FPreTreatedStaticMesh;
struct FPreTreatedSkyBox;
struct FPreTreatedRenderInformation;

struct FTreatedStaticMesh
{
	uint64_t mIndexInObjCb = UINT64_MAX;
	uint64_t mIndicesCount;

	D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
	D3D12_INDEX_BUFFER_VIEW mIndexBufferView;

	ID3D12RootSignature * mRootSignature;
	ID3D12PipelineState * mPipelineState;

	std::vector<FShaderParameter> mShaderParameters;

	FTreatedStaticMesh() = default;
	FTreatedStaticMesh(const FPreTreatedStaticMesh & _ptsm);
};

struct FTreatedSkyBox
{
	uint64_t mIndicesCount;

	D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
	D3D12_INDEX_BUFFER_VIEW mIndexBufferView;

	ID3D12RootSignature * mRootSignature;
	ID3D12PipelineState * mPipelineState;

	std::vector<FShaderParameter> mShaderParameters;

	FTreatedSkyBox() = default;
	FTreatedSkyBox(const FPreTreatedSkyBox & _ptSkyBox);
};

struct FTreatedRenderInformation
{
	ID3D12Resource * mRenderTarget;
	D3D12_CPU_DESCRIPTOR_HANDLE mRenderTargetView;
	D3D12_VIEWPORT mD3DViewport;
	D3D12_RECT mScissorRect;

	ID3D12Resource * mDepthStencilBuffer;
	D3D12_CPU_DESCRIPTOR_HANDLE mDepthStencilView;

	FUploadConstantBuffer<PassCb> * mMainPassCb;
	FUploadConstantBuffer<ObjectCb> * mObjCb;
	FTreatedSkyBox mTreatedSkyBox;
	std::vector<FTreatedStaticMesh> mTreatedStaticMeshs;

	FTreatedRenderInformation() = default;
	FTreatedRenderInformation(const FPreTreatedRenderInformation & _ptri, FGpuRenderFrameResource * _gpuRenderFrameResource);
};
