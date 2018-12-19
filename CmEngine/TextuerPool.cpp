#include "TextuerPool.h"
#include "d3dx12.h"
#include "D3D12RhiGlobal.h"
#include "D3D12RhiUtil.h"

ID3D12Resource * FTexturePool::AllocateTexture(D3D12_RESOURCE_DESC _desc, D3D12_RESOURCE_STATES _state, const D3D12_CLEAR_VALUE * _clearValue)
{
	ID3D12Resource * newResource = nullptr;
	VerifyD3D12Result(GDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&_desc,
		_state,
		_clearValue,
		IID_PPV_ARGS(&newResource)
	));
	return newResource;
}

void FTexturePool::ReleaseTexture(ID3D12Resource * _texture)
{
	_texture->Release();
}
