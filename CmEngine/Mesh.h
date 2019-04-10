#pragma once
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <d3d12.h>
using namespace DirectX;

class UMesh
{
public:
	void Init(std::wstring _fileName);
	void CommitResource();
	void EndCommit();
	void Clear();

	__forceinline D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView()const
	{
		D3D12_VERTEX_BUFFER_VIEW returned;
		returned.BufferLocation = mVertexesGpu->GetGPUVirtualAddress();
		returned.StrideInBytes = mVertexSize;
		returned.SizeInBytes = mVertexSize * mVertexesCount;
		return returned;
	}

	__forceinline D3D12_INDEX_BUFFER_VIEW GetIndexBufferView()const
	{
		D3D12_INDEX_BUFFER_VIEW returned;
		returned.BufferLocation = mIndicesGpu->GetGPUVirtualAddress();
		returned.Format = mIndexFormat;
		returned.SizeInBytes = mIndexSize * mIndicesCount;
		return returned;
	}

	uint64_t GetVertexId()const { return mVertexId; }
	uint64_t GetIndicesCount()const { return mIndicesCount; }
	float GetBoundSphereRadius()const { return mBoundSphereRadius; }

private:
	uint64_t mVertexId;

	uint32_t mVertexSize = 0;
	uint32_t mVertexesCount = 0;
	DXGI_FORMAT mIndexFormat = DXGI_FORMAT_UNKNOWN;
	uint32_t mIndexSize = 0;
	uint32_t mIndicesCount = 0;

	float mBoundSphereRadius = 0.0f;

	void * mVertexesBuffer = nullptr;
	void * mIndicesBuffer = nullptr;

	ID3D12Resource * mVertexesGpu = nullptr;
	ID3D12Resource * mIndicesGpu = nullptr;
	ID3D12Resource * mUploadVertexesBuffer = nullptr;
	ID3D12Resource * mUploadIndicesBuffer = nullptr;
};
