#pragma once
#include <string>
#include <d3d12.h>
#include <set>
#include <vector>
#include "ShaderParameter.h"

class UMaterials
{
public:
	void Init(std::wstring _fileName);
	void Clear();

	__forceinline ID3D12PipelineState * GetPipelineState() { return mPipelineState; }
	__forceinline ID3D12RootSignature * GetRootSignature() { return mRootSignature; }

	__forceinline const std::vector<D3D12_ROOT_PARAMETER_TYPE> & GetParameterIdentifications()const
	{
		return mParameterIdentifications;
	}

	bool IsFittedVertexId(uint64_t _vertexId)
	{
		return mFitVertexIds.find(_vertexId) != mFitVertexIds.end();
	}

private:
	std::set<uint64_t> mFitVertexIds;
	std::vector<D3D12_ROOT_PARAMETER_TYPE> mParameterIdentifications;
	ID3D12PipelineState * mPipelineState;
	ID3D12RootSignature * mRootSignature;
};
