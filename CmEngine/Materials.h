#pragma once
#include <string>
#include <d3d12.h>
#include <set>
#include <vector>
#include "ShaderParameter.h"
#include "EEntityType.h"

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
		return mFitVertexIds.count(_vertexId) != 0;
	}

	bool IsFittedEntityType(EEntityType _entityType)
	{
		return mFitEntityTypes.count(_entityType) != 0;
	}

private:
	std::set<uint64_t> mFitVertexIds;
	std::set<EEntityType> mFitEntityTypes;
	std::vector<D3D12_ROOT_PARAMETER_TYPE> mParameterIdentifications;
	ID3D12PipelineState * mPipelineState;
	ID3D12RootSignature * mRootSignature;
};
