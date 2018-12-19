#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include "ConstantBufferSruct.h"
#include <vector>

class FGpuRenderFrameResource;
struct FUntreatedRenderInformation;
struct FUntreatedRenderStaticMesh;

struct FPreTreatedRenderStaticMesh
{
	ObjectCb mObjCb;

	uint64_t mIndicesCount;
	D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
	D3D12_INDEX_BUFFER_VIEW mIndexBufferView;

	ID3D12RootSignature * mRootSignature;
	ID3D12PipelineState * mPipelineState;

	FPreTreatedRenderStaticMesh() = default;
	FPreTreatedRenderStaticMesh(const FUntreatedRenderStaticMesh & _utsm);
};

struct FPreTreatedRenderInformation
{
	ID3D12Resource * mRenderTarget;
	D3D12_CPU_DESCRIPTOR_HANDLE mRenderTargetView;
	D3D12_VIEWPORT mD3DViewport;
	D3D12_RECT mScissorRect;

	PassCb mPassCb;

	std::vector<FPreTreatedRenderStaticMesh> mPreUreatedRenderStaticMeshs;

	FPreTreatedRenderInformation() = default;
	FPreTreatedRenderInformation(const FUntreatedRenderInformation & _utri, FGpuRenderFrameResource * _gpuRenderFrameResource);
};
