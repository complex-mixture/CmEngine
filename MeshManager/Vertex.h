#pragma once
#include <DirectXMath.h>

enum class EVertexElementFlag : uint64_t
{
	Position = 1 << 0,
	Color = 1 << 1,
	UV = 1 << 2,
	Targent = 1 << 3,
	Binormal = 1 << 4,
	NumBits = 5,
	DefaultFlag = (1 << 5) - 1
};

struct Vertex_default
{
	static constexpr uint64_t VertexId = 1;

	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT2 mUV;
	DirectX::XMFLOAT3 mTangent;
	DirectX::XMFLOAT3 mBinormal;
};
