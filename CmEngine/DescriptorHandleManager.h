#pragma once
#include <d3d12.h>
#include <stdint.h>
#include "Util.h"
#include "DescriptorHandleManagerBase.h"
#include <intsafe.h>
#include "D3D12RhiUtil.h"
#include "D3D12RhiGlobal.h"

template <typename _descT>
class FDescriptorHandleManager : public FDescriptorHandleManagerBase
{
	FDescriptorHandleManager() = default;

	template<typename... _argts>
	void __CreateView(_argts... _args);

public:
	using DescType = _descT;
	static FDescriptorHandleManager & Get()
	{
		static FDescriptorHandleManager single;
		return single;
	}

	/*
	ConstantBufferView : _desc
	DepthStencilView : _buffer, _desc
	RenderTargetView : _renderTarget, _Desc
	ShaderResourceView : _resource, _Desc
	UnorderedAccessView : _resource, _counterResource, _desc
	*/
	template<typename... _argts>
	FDescriptorHandle CreateView(_argts... _args)
	{
		uint32_t slot = FindFreeSlot();
		Assert(slot != UINT32_ERROR);
		FDescriptorHandle returned = mDescriptorHandle;
		returned.Offset(slot * mDescriptorHandleSize);
		__CreateView(_args..., returned.mCpuHandle);
		MarkUsedSlot(slot);
		return returned;
	}
};

template<typename _descT>
template<typename ..._argts>
inline void FDescriptorHandleManager<_descT>::__CreateView(_argts ..._args) = delete;

template<>
template<>
inline void FDescriptorHandleManager<D3D12_CONSTANT_BUFFER_VIEW_DESC>::__CreateView(
	D3D12_CONSTANT_BUFFER_VIEW_DESC  _desc,
	D3D12_CPU_DESCRIPTOR_HANDLE _destHandle
)
{
	GDevice->CreateConstantBufferView(&_desc, _destHandle);
}

template<>
template<>
inline void FDescriptorHandleManager<D3D12_DEPTH_STENCIL_VIEW_DESC>::__CreateView(
	ID3D12Resource * _depthStencilBuffer,
	D3D12_DEPTH_STENCIL_VIEW_DESC  _desc,
	D3D12_CPU_DESCRIPTOR_HANDLE _destHandle
)
{
	GDevice->CreateDepthStencilView(_depthStencilBuffer, &_desc, _destHandle);
}

template<>
template<>
inline void FDescriptorHandleManager<D3D12_RENDER_TARGET_VIEW_DESC >::__CreateView(
	ID3D12Resource * _renderTarget,
	D3D12_RENDER_TARGET_VIEW_DESC _desc,
	D3D12_CPU_DESCRIPTOR_HANDLE _destHandle
)
{
	GDevice->CreateRenderTargetView(_renderTarget, &_desc, _destHandle);
}

template<>
template<>
inline void FDescriptorHandleManager<D3D12_SHADER_RESOURCE_VIEW_DESC>::__CreateView(
	ID3D12Resource * _resource,
	D3D12_SHADER_RESOURCE_VIEW_DESC _desc,
	D3D12_CPU_DESCRIPTOR_HANDLE _destHandle
)
{
	GDevice->CreateShaderResourceView(_resource, &_desc, _destHandle);
}


template<>
template<>
inline void FDescriptorHandleManager<D3D12_UNORDERED_ACCESS_VIEW_DESC >::__CreateView(
	ID3D12Resource * _resource,
	ID3D12Resource * _counterResource,
	D3D12_UNORDERED_ACCESS_VIEW_DESC  _desc,
	D3D12_CPU_DESCRIPTOR_HANDLE _destHandle
)
{
	GDevice->CreateUnorderedAccessView(_resource, _counterResource, &_desc, _destHandle);
}
