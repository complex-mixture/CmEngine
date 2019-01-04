#pragma once
#include <d3d12.h>
#include <stdint.h>
#include <vector>

struct FShaderParameter
{
	D3D12_ROOT_PARAMETER_TYPE mParameterType;
	union
	{
		D3D12_GPU_DESCRIPTOR_HANDLE mParameterDescriptorTable;
		D3D12_GPU_VIRTUAL_ADDRESS mParameterConstantBufferView;
		D3D12_GPU_VIRTUAL_ADDRESS mParameterShaderResourcedView;
		D3D12_GPU_VIRTUAL_ADDRESS mParameterUnorderedAccessView;
	};
};
