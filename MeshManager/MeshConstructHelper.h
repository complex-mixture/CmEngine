#pragma once
#include <stdint.h>
#include "Util.h"
#include <DirectXMath.h>
#include <dxgiformat.h>
#include <fbxsdk.h>

inline void ConstructIndices(_In_ uint64_t _vertexCount, _Out_ void * _vertexBuffer, _In_ DXGI_FORMAT _indexFormat)
{
	Assert(_vertexBuffer);

	switch (_indexFormat)
	{
	case DXGI_FORMAT_R16_UINT:
	{
		uint16_t * indicesBuffer = reinterpret_cast<uint16_t*>(_vertexBuffer);
		for (size_t i = 0; i != _vertexCount; ++i)
			indicesBuffer[i] = i;
	}
	break;
	case DXGI_FORMAT_R32_UINT:
	{
		uint32_t * indicesBuffer = reinterpret_cast<uint32_t*>(_vertexBuffer);
		for (size_t i = 0; i != _vertexCount; ++i)
			indicesBuffer[i] = i;
	}
	break;
	default:
		Assert(false);
	}
}

template<typename _vertexType>
void ConstructVertexPositions(_In_ uint64_t _vertexesCount, _Out_ _vertexType * _vertexes, _In_ const FbxVector4 * _positions, _In_ const int * _indexes, DirectX::XMFLOAT3 _vertexType::* _positionMember)
{
	Assert(_vertexes && _positions && _indexes);

	for (size_t i = 0; i != _vertexesCount; ++i)
	{
		(_vertexes[i].*_positionMember).x = _positions[_indexes[i]][0];
		(_vertexes[i].*_positionMember).y = _positions[_indexes[i]][1];
		(_vertexes[i].*_positionMember).z = _positions[_indexes[i]][2];
	}
}

template<typename _vertexType>
void ConstructVertexUvs(_In_ uint64_t _vertexesCount, _Out_ _vertexType * _vertexes, _In_ const FbxGeometryElementUV * _uvs, bool _isExportFromUnreal, DirectX::XMFLOAT2 _vertexType::* _uvMember)
{
	switch (_uvs->GetReferenceMode())
	{
	case FbxLayerElement::eDirect:
	{
		const FbxLayerElementArrayTemplate<FbxVector2> & directArray = _uvs->GetDirectArray();
		LogA("uv count : %d\n", directArray.GetCount());
		if (_isExportFromUnreal)
		{
			for (size_t i = 0; i != _vertexesCount; ++i)
			{
				(_vertexes[i].*_uvMember).x = directArray[i][0];
				(_vertexes[i].*_uvMember).y = 1 - directArray[i][1];
			}
		}
		else
		{
			for (size_t i = 0; i != _vertexesCount; ++i)
			{
				(_vertexes[i].*_uvMember).x = directArray[i][0];
				(_vertexes[i].*_uvMember).y = directArray[i][1];
			}
		}
	}
	break;
	case FbxLayerElement::eIndex:
	case FbxLayerElement::eIndexToDirect:
	{
		const FbxLayerElementArrayTemplate<FbxVector2> & directArray = _uvs->GetDirectArray();
		const FbxLayerElementArrayTemplate<int> & indicesArray = _uvs->GetIndexArray();
		LogA("uv count : %d\n", indicesArray.GetCount());

		if (_isExportFromUnreal)
		{
			for (size_t i = 0; i != _vertexesCount; ++i)
			{
				int index = indicesArray[i];
				(_vertexes[i].*_uvMember).x = directArray[index][0];
				(_vertexes[i].*_uvMember).y = 1 - directArray[index][1];
			}
		}
		else
		{
			for (size_t i = 0; i != _vertexesCount; ++i)
			{
				int index = indicesArray[i];
				(_vertexes[i].*_uvMember).x = directArray[index][0];
				(_vertexes[i].*_uvMember).y = directArray[index][1];
			}
		}
	}
	break;
	default: Assert(false);
	}
}

template<typename _vertexType>
void ConstructVertexTangent(_In_ uint64_t _vertexesCount, _Out_ _vertexType * _vertexes, _In_ const FbxGeometryElementTangent * _tangents, DirectX::XMFLOAT3 _vertexType::* _tangentMember)
{
	switch (_tangents->GetReferenceMode())
	{
	case FbxLayerElement::eDirect:
	{
		const FbxLayerElementArrayTemplate<FbxVector4> & directArray = _tangents->GetDirectArray();
		LogA("tangents count : %d\n", directArray.GetCount());

		for (size_t i = 0; i != _vertexesCount; ++i)
		{
			(_vertexes[i].*_tangentMember).x = directArray[i][0];
			(_vertexes[i].*_tangentMember).y = directArray[i][1];
			(_vertexes[i].*_tangentMember).z = directArray[i][2];
		}
	}
	break;
	case FbxLayerElement::eIndex:
	case FbxLayerElement::eIndexToDirect:
	{
		const FbxLayerElementArrayTemplate<FbxVector4> & directArray = _tangents->GetDirectArray();
		const FbxLayerElementArrayTemplate<int> & indicesArray = _tangents->GetIndexArray();
		LogA("tangents count : %d\n", indicesArray.GetCount());

		for (size_t i = 0; i != _vertexesCount; ++i)
		{
			int index = indicesArray[i];
			(_vertexes[i].*_tangentMember).x = directArray[index][0];
			(_vertexes[i].*_tangentMember).y = directArray[index][1];
			(_vertexes[i].*_tangentMember).z = directArray[index][2];
		}
	}
	break;
	default: Assert(false);
	}
}

template<typename _vertexType>
void ConstructVertexBinormal(_In_ uint64_t _vertexesCount, _Out_ _vertexType * _vertexes, _In_ const FbxGeometryElementBinormal * _binormal, DirectX::XMFLOAT3 _vertexType::* _binormalMember)
{
	switch (_binormal->GetReferenceMode())
	{
	case FbxLayerElement::eDirect:
	{
		const FbxLayerElementArrayTemplate<FbxVector4> & directArray = _binormal->GetDirectArray();
		LogA("tangents count : %d\n", directArray.GetCount());

		for (size_t i = 0; i != _vertexesCount; ++i)
		{
			(_vertexes[i].*_binormalMember).x = directArray[i][0];
			(_vertexes[i].*_binormalMember).y = directArray[i][1];
			(_vertexes[i].*_binormalMember).z = directArray[i][2];
		}				  
	}
	break;
	case FbxLayerElement::eIndex:
	case FbxLayerElement::eIndexToDirect:
	{
		const FbxLayerElementArrayTemplate<FbxVector4> & directArray = _binormal->GetDirectArray();
		const FbxLayerElementArrayTemplate<int> & indicesArray = _binormal->GetIndexArray();
		LogA("tangents count : %d\n", indicesArray.GetCount());

		for (size_t i = 0; i != _vertexesCount; ++i)
		{
			int index = indicesArray[i];
			(_vertexes[i].*_binormalMember).x = directArray[index][0];
			(_vertexes[i].*_binormalMember).y = directArray[index][1];
			(_vertexes[i].*_binormalMember).z = directArray[index][2];
		}
	}
	break;
	default: Assert(false);
	}
}