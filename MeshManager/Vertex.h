#pragma once
#include <DirectXMath.h>

struct Vertex_default
{
	static constexpr uint64_t VertexId = 0;

	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT2 mUV;
	DirectX::XMFLOAT3 mTangent;
	DirectX::XMFLOAT3 mBinormal;
};

struct Vertex_SkyBox
{
	static constexpr uint64_t VertexId = 1;

	DirectX::XMFLOAT3 mPosition;
};
