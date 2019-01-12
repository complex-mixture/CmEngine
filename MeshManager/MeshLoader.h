#pragma once
#include "Util.h"
#include "MeshConstructHelper.h"
#include "StaticMesh.h"
#include "Vertex.h"
#include <fbxsdk.h>

template<>
inline void UStaticMesh<Vertex_default>::__Contruct(const FbxMesh * _mesh, bool _isExportFromUnreal)
{
	ConstructIndices(mIndicesCount, mIndicesBuffer, mIndexFormat);
	ConstructVertexPositions(mVertexesCount, &mVertexes[0], _mesh->GetControlPoints(), _mesh->GetPolygonVertices(), &Vertex_default::mPosition);
	ConstructVertexUvs(mVertexesCount, &mVertexes[0], _mesh->GetElementUV(), _isExportFromUnreal, &Vertex_default::mUV);
	ConstructVertexTangent(mVertexesCount, &mVertexes[0], _mesh->GetElementTangent(), &Vertex_default::mTangent);
	ConstructVertexBinormal(mVertexesCount, &mVertexes[0], _mesh->GetElementBinormal(), &Vertex_default::mBinormal);
}

template<>
inline void UStaticMesh<Vertex_SkyBox>::__Contruct(const FbxMesh * _mesh, bool _isExportFromUnreal)
{
	ConstructIndices(mIndicesCount, mIndicesBuffer, mIndexFormat);
	ConstructVertexPositions(mVertexesCount, &mVertexes[0], _mesh->GetControlPoints(), _mesh->GetPolygonVertices(), &Vertex_SkyBox::mPosition);
}