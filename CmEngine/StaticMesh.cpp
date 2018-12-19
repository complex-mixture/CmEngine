#include "StaticMesh.h"
#include "d3dx12.h"
#include <fstream>
#include "D3D12RhiGlobal.h"
#include "D3D12RhiUtil.h"

void UStaticMesh::Init(std::wstring _fileName)
{
	std::ifstream fs(_fileName, std::ios::in | std::ios::binary);

	fs.read(reinterpret_cast<char*>(&mVertexId), sizeof(uint64_t));
	fs.read(reinterpret_cast<char*>(&mVertexSize), sizeof(uint32_t));
	fs.read(reinterpret_cast<char*>(&mVertexesCount), sizeof(uint32_t));
	fs.read(reinterpret_cast<char*>(&mIndexFormat), sizeof(DXGI_FORMAT));
	fs.read(reinterpret_cast<char*>(&mIndexSize), sizeof(uint32_t));
	fs.read(reinterpret_cast<char*>(&mIndicesCount), sizeof(uint32_t));

	mVertexesBuffer = malloc(mVertexSize * mVertexesCount);
	mIndicesBuffer = malloc(mIndexSize * mIndicesCount);

	fs.read(reinterpret_cast<char*>(mVertexesBuffer), mVertexSize * mVertexesCount);
	fs.read(reinterpret_cast<char*>(mIndicesBuffer), mIndexSize * mIndicesCount);
	
	fs.close();
}

void UStaticMesh::CommitResource()
{
	VerifyD3D12Result(GDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(mVertexSize * mVertexesCount),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&mVertexesGpu)
	));

	VerifyD3D12Result(GDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(mIndexSize * mIndicesCount),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&mIndicesGpu)
	));

	VerifyD3D12Result(GDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(mVertexSize * mVertexesCount),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mUploadVertexesBuffer)
	));

	VerifyD3D12Result(GDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(mIndexSize * mIndicesCount),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mUploadIndicesBuffer)
	));

	D3D12_RESOURCE_BARRIER resourceBarrierBefore[] = {
		CD3DX12_RESOURCE_BARRIER::Transition(mVertexesGpu, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST),
		CD3DX12_RESOURCE_BARRIER::Transition(mIndicesGpu, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST),
	};

	GCommandList->ResourceBarrier(_countof(resourceBarrierBefore), resourceBarrierBefore);

	D3D12_SUBRESOURCE_DATA subResourceDataVertexes = {};
	subResourceDataVertexes.pData = mVertexesBuffer;
	subResourceDataVertexes.RowPitch = mVertexSize * mVertexesCount;
	subResourceDataVertexes.SlicePitch = mVertexSize * mVertexesCount;

	UpdateSubresources<1>(GCommandList, mVertexesGpu, mUploadVertexesBuffer, 0, 0, 1, &subResourceDataVertexes);

	D3D12_SUBRESOURCE_DATA subResourceDataIndices = {};
	subResourceDataIndices.pData = mIndicesBuffer;
	subResourceDataIndices.RowPitch = mIndexSize * mIndicesCount;
	subResourceDataIndices.SlicePitch = mIndexSize * mIndicesCount;

	UpdateSubresources<1>(GCommandList, mIndicesGpu, mUploadIndicesBuffer, 0, 0, 1, &subResourceDataIndices);


	D3D12_RESOURCE_BARRIER resourceBarrierAfter[] = {
		CD3DX12_RESOURCE_BARRIER::Transition(mVertexesGpu, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COMMON),
		CD3DX12_RESOURCE_BARRIER::Transition(mIndicesGpu, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COMMON),
	};

	GCommandList->ResourceBarrier(_countof(resourceBarrierAfter), resourceBarrierAfter);

	free(mVertexesBuffer);
	free(mIndicesBuffer);
	mVertexesBuffer = nullptr;
	mIndicesBuffer = nullptr;
}

void UStaticMesh::EndCommit()
{
	mUploadVertexesBuffer->Release();
	mUploadVertexesBuffer = nullptr;
	mUploadIndicesBuffer->Release();
	mUploadIndicesBuffer = nullptr;
}

void UStaticMesh::Clear()
{
	if (mVertexesGpu != nullptr)
		mVertexesGpu->Release();
	if (mIndicesGpu != nullptr)
		mIndicesGpu->Release();
	mVertexesGpu = nullptr;
	mIndicesGpu = nullptr;
}
