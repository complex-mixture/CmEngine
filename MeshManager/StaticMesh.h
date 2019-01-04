#pragma once
#include <dxgiformat.h>
#include <fbxsdk.h>
#include "Util.h"
#include <vector>

template<typename _vertexType>
class UStaticMesh
{
	void __Contruct(const FbxMesh * _mesh);
public:
	UStaticMesh() = default;
	void Construct(const FbxMesh * _mesh)
	{
		mIndicesCount = _mesh->GetPolygonVertexCount();
		mVertexesCount = mIndicesCount;
		if (mIndicesCount <= UINT16_MAX)
		{
			mIndexSize = sizeof(uint16_t);
			mIndexFormat = DXGI_FORMAT_R16_UINT;
			mIndicesBuffer = new uint16_t[mIndicesCount];
		}
		else if (mIndicesCount <= UINT32_MAX)
		{
			mIndexSize = sizeof(uint32_t);
			mIndexFormat = DXGI_FORMAT_R32_UINT;
			mIndicesBuffer = new uint32_t[mIndicesCount];
		}
		else
		{
			DebugMessageBoxW(L"too much Indices", L"only suport %ud or lesser Indices, but there is %ud indices", UINT32_MAX, mIndicesCount);
		}
		if (_mesh->GetPolygonSize(0) != 3)
			DebugMessageBoxW(L"Mesh error", L"the mesh Polygon should be tri");
		mVertexes.resize(mIndicesCount);
		__Contruct(_mesh);
	}
	void SaveToFile(const std::wstring & _fileName)
	{
		std::ofstream ofs(_fileName, std::ios::out | std::ios::binary);

		ofs.write(reinterpret_cast<const char*>(&mVertexId), sizeof(uint64_t));
		ofs.write(reinterpret_cast<const char*>(&mVertexSize), sizeof(uint32_t));
		ofs.write(reinterpret_cast<const char*>(&mVertexesCount), sizeof(uint32_t));
		ofs.write(reinterpret_cast<const char*>(&mIndexFormat), sizeof(DXGI_FORMAT));
		ofs.write(reinterpret_cast<const char*>(&mIndexSize), sizeof(uint32_t));
		ofs.write(reinterpret_cast<const char*>(&mIndicesCount), sizeof(uint32_t));

		ofs.write(reinterpret_cast<const char*>(mVertexes.data()), mVertexSize * mVertexesCount);
		ofs.write(reinterpret_cast<const char*>(mIndicesBuffer), mIndexSize * mIndicesCount);
		ofs.close();
	}
	~UStaticMesh() { mVertexes.resize(0); delete[] mIndicesBuffer; }
private:
	uint64_t mVertexId = _vertexType::VertexId;

	uint32_t mVertexSize = sizeof(_vertexType);
	uint32_t mVertexesCount;
	DXGI_FORMAT mIndexFormat;
	uint32_t mIndexSize;
	uint32_t mIndicesCount;

	std::vector<_vertexType> mVertexes;
	void * mIndicesBuffer = nullptr;
};

template<typename _vertexType>
inline void UStaticMesh<_vertexType>::__Contruct(const FbxMesh * _mesh) = delete;

#include "MeshLoader.inl"
