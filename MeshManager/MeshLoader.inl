#include "Util.h"
template<>
inline void UStaticMesh<Vertex_default>::__Contruct(const FbxMesh * _mesh)
{
	int * indices = _mesh->GetPolygonVertices();
	LogA("%s : polygon size [%d]\n", _mesh->GetName(), _mesh->GetPolygonSize(0));
	LogA("%s : control points count [%d]\n", _mesh->GetName(), _mesh->GetControlPointsCount());

	switch (mIndexFormat)
	{
	case DXGI_FORMAT_R16_UINT:
	{
		uint16_t * indicesBuffer = reinterpret_cast<uint16_t*>(mIndicesBuffer);
		for (size_t i = 0; i != mIndicesCount; ++i)
			indicesBuffer[i] = i;
	}
	break;
	case DXGI_FORMAT_R32_UINT:
	{
		uint32_t * indicesBuffer = reinterpret_cast<uint32_t*>(mIndicesBuffer);
		for (size_t i = 0; i != mIndicesCount; ++i)
			indicesBuffer[i] = i;
	}
	break;
	default:
		Assert(false);
	}

	const FbxVector4* vertexs = _mesh->GetControlPoints();
	//const FbxGeometryElementVertexColor * colors = _mesh->GetElementVertexColor();
	const FbxGeometryElementUV * uvs = _mesh->GetElementUV();
	const FbxGeometryElementTangent * tangents = _mesh->GetElementTangent();
	const FbxGeometryElementBinormal * binormals = _mesh->GetElementBinormal();

	Assert(vertexs && uvs && tangents && binormals);

	for (size_t i = 0; i != mVertexesCount; ++i)
	{
		mVertexes[i].mPosition.x = vertexs[indices[i]][0];
		mVertexes[i].mPosition.y = vertexs[indices[i]][1];
		mVertexes[i].mPosition.z = vertexs[indices[i]][2];
	}

	//switch (colors->GetReferenceMode())
	//{
	//case FbxLayerElement::eDirect:
	//{
	//	const FbxLayerElementArrayTemplate<FbxColor> & directArray = colors->GetDirectArray();

	//	for (size_t i = 0; i != mVertexesCount; ++i)
	//	{
	//		mVertexes[i].mColor.x = directArray[i].mRed;
	//		mVertexes[i].mColor.y = directArray[i].mGreen;
	//		mVertexes[i].mColor.z = directArray[i].mBlue;
	//		mVertexes[i].mColor.w = directArray[i].mAlpha;
	//	}
	//}
	//break;
	//case FbxLayerElement::eIndex:
	//case FbxLayerElement::eIndexToDirect:
	//{
	//	const FbxLayerElementArrayTemplate<FbxColor> & directArray = colors->GetDirectArray();
	//	const FbxLayerElementArrayTemplate<int> & indicesArray = colors->GetIndexArray();

	//	for (size_t i = 0; i != mVertexesCount; ++i)
	//	{
	//		int index = indicesArray[i];
	//		mVertexes[i].mColor.x = directArray[index].mRed;
	//		mVertexes[i].mColor.y = directArray[index].mGreen;
	//		mVertexes[i].mColor.z = directArray[index].mBlue;
	//		mVertexes[i].mColor.w = directArray[index].mAlpha;
	//	}
	//}
	//break;
	//default: Assert(false);
	//}

	switch (uvs->GetReferenceMode())
	{
	case FbxLayerElement::eDirect:
	{
		const FbxLayerElementArrayTemplate<FbxVector2> & directArray = uvs->GetDirectArray();
		LogA("uv count : %d\n", directArray.GetCount());
		for (size_t i = 0; i != mVertexesCount; ++i)
		{
			mVertexes[i].mUV.x = directArray[i][0];
			mVertexes[i].mUV.y = directArray[i][1];
		}
	}
	break;
	case FbxLayerElement::eIndex:
	case FbxLayerElement::eIndexToDirect:
	{
		const FbxLayerElementArrayTemplate<FbxVector2> & directArray = uvs->GetDirectArray();
		const FbxLayerElementArrayTemplate<int> & indicesArray = uvs->GetIndexArray();
		LogA("uv count : %d\n", indicesArray.GetCount());

		for (size_t i = 0; i != mVertexesCount; ++i)
		{
			int index = indicesArray[i];
			mVertexes[i].mUV.x = directArray[index][0];
			mVertexes[i].mUV.y = directArray[index][1];
		}
	}
	break;
	default: Assert(false);
	}

	switch (tangents->GetReferenceMode())
	{
	case FbxLayerElement::eDirect:
	{
		const FbxLayerElementArrayTemplate<FbxVector4> & directArray = tangents->GetDirectArray();
		LogA("tangents count : %d\n", directArray.GetCount());

		for (size_t i = 0; i != mVertexesCount; ++i)
		{
			mVertexes[i].mTangent.x = directArray[i][0];
			mVertexes[i].mTangent.y = directArray[i][1];
			mVertexes[i].mTangent.z = directArray[i][2];
		}
	}
	break;
	case FbxLayerElement::eIndex:
	case FbxLayerElement::eIndexToDirect:
	{
		const FbxLayerElementArrayTemplate<FbxVector4> & directArray = tangents->GetDirectArray();
		const FbxLayerElementArrayTemplate<int> & indicesArray = tangents->GetIndexArray();
		LogA("tangents count : %d\n", indicesArray.GetCount());

		for (size_t i = 0; i != mVertexesCount; ++i)
		{
			int index = indicesArray[i];
			mVertexes[i].mTangent.x = directArray[index][0];
			mVertexes[i].mTangent.y = directArray[index][1];
			mVertexes[i].mTangent.z = directArray[index][2];
		}
	}
	break;
	default: Assert(false);
	}

	switch (binormals->GetReferenceMode())
	{
	case FbxLayerElement::eDirect:
	{
		const FbxLayerElementArrayTemplate<FbxVector4> & directArray = binormals->GetDirectArray();
		LogA("binormals count : %d\n", directArray.GetCount());

		for (size_t i = 0; i != mVertexesCount; ++i)
		{
			mVertexes[i].mBinormal.x = directArray[i][0];
			mVertexes[i].mBinormal.y = directArray[i][1];
			mVertexes[i].mBinormal.z = directArray[i][2];
		}
	}
	break;
	case FbxLayerElement::eIndex:
	case FbxLayerElement::eIndexToDirect:
	{
		const FbxLayerElementArrayTemplate<FbxVector4> & directArray = binormals->GetDirectArray();
		const FbxLayerElementArrayTemplate<int> & indicesArray = binormals->GetIndexArray();
		LogA("binormals count : %d\n", indicesArray.GetCount());

		for (size_t i = 0; i != mVertexesCount; ++i)
		{
			int index = indicesArray[i];
			mVertexes[i].mBinormal.x = directArray[index][0];
			mVertexes[i].mBinormal.y = directArray[index][1];
			mVertexes[i].mBinormal.z = directArray[index][2];
		}
	}
	break;
	default: Assert(false);
	}
}
