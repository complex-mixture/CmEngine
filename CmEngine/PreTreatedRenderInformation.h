#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include "ConstantBufferSruct.h"
#include <vector>
#include "ShaderParameter.h"

class FGpuRenderFrameResource;
struct FUntreatedRenderInformation;
struct FUntreatedStaticMesh;
struct FUntreatedSkyBox;

struct FPreTreatedSkyBox
{
	uint64_t mIndicesCount;
	D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
	D3D12_INDEX_BUFFER_VIEW mIndexBufferView;

	ID3D12RootSignature * mRootSignature;
	ID3D12PipelineState * mPipelineState;

	std::vector<FShaderParameter> mShaderParameters;

	FPreTreatedSkyBox() = default;
	FPreTreatedSkyBox(const FUntreatedSkyBox & _utskybox);
};

struct FPreTreatedStaticMesh
{
	ObjectCb mObjCb;

	uint64_t mIndicesCount;
	D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
	D3D12_INDEX_BUFFER_VIEW mIndexBufferView;

	ID3D12RootSignature * mRootSignature;
	ID3D12PipelineState * mPipelineState;

	std::vector<FShaderParameter> mShaderParameters;

	FPreTreatedStaticMesh() = default;
	FPreTreatedStaticMesh(const FUntreatedStaticMesh & _utsm);
};


struct FPreTreatedRenderInformation
{
	ID3D12Resource * mRenderTarget;
	D3D12_CPU_DESCRIPTOR_HANDLE mRenderTargetView;
	D3D12_VIEWPORT mD3DViewport;
	D3D12_RECT mScissorRect;

	PassCb mPassCb;

	FPreTreatedSkyBox mPtSkyBox;
	std::vector<FPreTreatedStaticMesh> mPtStaticMeshs;

	FPreTreatedRenderInformation() = default;
	FPreTreatedRenderInformation(const FUntreatedRenderInformation & _utri, FGpuRenderFrameResource * _gpuRenderFrameResource);
};
