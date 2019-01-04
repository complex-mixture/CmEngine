#pragma once
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <d3d12.h>
using namespace DirectX;

class UStaticMesh
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

private:
	uint64_t mVertexId;

	uint32_t mVertexSize;
	uint32_t mVertexesCount;
	DXGI_FORMAT mIndexFormat;
	uint32_t mIndexSize;
	uint32_t mIndicesCount;

	void * mVertexesBuffer = nullptr;
	void * mIndicesBuffer = nullptr;

	ID3D12Resource * mVertexesGpu = nullptr;
	ID3D12Resource * mIndicesGpu = nullptr;
	ID3D12Resource * mUploadVertexesBuffer = nullptr;
	ID3D12Resource * mUploadIndicesBuffer = nullptr;
};
