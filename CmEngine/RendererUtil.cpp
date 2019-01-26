#include "RendererUtil.h"
#include "D3D12RhiGlobal.h"

void SetShaderParameters(const std::vector<FShaderParameter>& _parameters, uint64_t _startIndex)
{
	for (uint64_t i = 0; i != _parameters.size(); ++i)
	{
		switch (_parameters[i].mParameterType)
		{
		case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
			GCommandList->SetGraphicsRootDescriptorTable(i + _startIndex, _parameters[i].mParameterDescriptorTable);
			break;
		case D3D12_ROOT_PARAMETER_TYPE_CBV:
			GCommandList->SetComputeRootConstantBufferView(i + _startIndex, _parameters[i].mParameterConstantBufferView);
			break;
		case D3D12_ROOT_PARAMETER_TYPE_SRV:
			GCommandList->SetGraphicsRootShaderResourceView(i + _startIndex, _parameters[i].mParameterShaderResourcedView);
			break;
		case D3D12_ROOT_PARAMETER_TYPE_UAV:
			GCommandList->SetGraphicsRootUnorderedAccessView(i + _startIndex, _parameters[i].mParameterUnorderedAccessView);
			break;
		}
	}
}