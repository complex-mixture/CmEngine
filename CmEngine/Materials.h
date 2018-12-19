#pragma once
#include <string>
#include <d3d12.h>
#include <set>

class UMaterials
{
public:
	void Init(std::wstring _fileName);
	void Clear();

	__forceinline ID3D12PipelineState * GetPipelineState() { return mPipelineState; }
	__forceinline ID3D12RootSignature * GetRootSignature() { return mRootSignature; }

	bool IsFittedVertexId(uint64_t _vertexId)
	{
		return mFitVertexId.find(_vertexId) != mFitVertexId.end();
	}

private:
	std::set<uint64_t> mFitVertexId;
	ID3D12PipelineState * mPipelineState;
	ID3D12RootSignature * mRootSignature;
};
