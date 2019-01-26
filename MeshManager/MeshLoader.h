#pragma once
#include "Util.h"
#include "MeshConstructHelper.h"
#include "Mesh.h"
#include "Vertex.h"
#include <fbxsdk.h>

template<>
inline void UMesh<Vertex_default>::__Contruct(const FbxMesh * _mesh, bool _isExportFromUnreal)
{
	ConstructVertexPositions(mVertexesCount, &mVertexes[0], _mesh->GetControlPoints(), _mesh->GetPolygonVertices(), &Vertex_default::mPosition);
	ConstructVertexUvs(mVertexesCount, &mVertexes[0], _mesh->GetElementUV(), _isExportFromUnreal, &Vertex_default::mUV);
	ConstructVertexTangent(mVertexesCount, &mVertexes[0], _mesh->GetElementTangent(), &Vertex_default::mTangent);
	ConstructVertexBinormal(mVertexesCount, &mVertexes[0], _mesh->GetElementBinormal(), &Vertex_default::mBinormal);
}

template<>
inline void UMesh<Vertex_SkyBox>::__Contruct(const FbxMesh * _mesh, bool _isExportFromUnreal)
{
	ConstructVertexPositions(mVertexesCount, &mVertexes[0], _mesh->GetControlPoints(), _mesh->GetPolygonVertices(), &Vertex_SkyBox::mPosition);
}
