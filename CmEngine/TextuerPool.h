#pragma once
#include <d3d12.h>
#include <stdint.h>

class FTexturePool
{
public:
	static FTexturePool & Get()
	{
		static FTexturePool single;
		return single;
	}
	ID3D12Resource * AllocateTexture(D3D12_RESOURCE_DESC _desc, D3D12_RESOURCE_STATES _state, const D3D12_CLEAR_VALUE * _clearValue);
	void ReleaseTexture(ID3D12Resource * _texture);
};
