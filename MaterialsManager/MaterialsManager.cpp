#include "MaterialsManager.h"
#include <fstream>
#include "Util.h"

void FMaterialsManager::Init()
{
	std::ifstream ifs(mManagerFileName, std::ios::in | std::ios::binary);
	if (ifs.is_open())
	{
		uint64_t elementCount = 0;
		ifs.read(reinterpret_cast<char*>(&elementCount), sizeof(uint64_t));
		std::wstring meshName;
		std::wstring fileName;
		for (uint64_t i = 0; i != elementCount; ++i)
		{
			LoadStringFromFile<wchar_t>(ifs, meshName);
			LoadStringFromFile<wchar_t>(ifs, fileName);
			mMap.insert(std::make_pair(meshName, fileName));
		}
	}
	else
	{
		LogW(L"File (%s) not exit", mManagerFileName.c_str());
	}
}

void FMaterialsManager::Clear()
{
	Save();
}

void FMaterialsManager::AddMaterials(
	std::function<void(
		_Out_ Comment(VertexId) uint64_t &,
		_Out_ std::vector<CD3DX12_ROOT_PARAMETER> &,
		_Out_ std::vector<CD3DX12_STATIC_SAMPLER_DESC> &,
		_Out_ D3D12_ROOT_SIGNATURE_FLAGS &,
		_Out_ D3D_ROOT_SIGNATURE_VERSION &,
		_Out_ Comment(VS) ID3DBlob *&,
		_Out_ Comment(HS) ID3DBlob *&,
		_Out_ Comment(DS) ID3DBlob *&,
		_Out_ Comment(GS) ID3DBlob *&,
		_Out_ Comment(PS) ID3DBlob *&,
		_Out_ D3D12_GRAPHICS_PIPELINE_STATE_DESC &
		)
	> _materialsInitFunc,
	const std::wstring & _materialsName,
	const std::wstring & _destFileName
)
{
	uint64_t vertexId;
	std::vector<CD3DX12_ROOT_PARAMETER> rootParameters;
	std::vector<CD3DX12_STATIC_SAMPLER_DESC> staticSamplerDescs;
	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlag = D3D12_ROOT_SIGNATURE_FLAG_NONE;
	D3D_ROOT_SIGNATURE_VERSION rootSignatureVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
	ID3DBlob * shaderBuffers[5] = {};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipeLineStateDesc;

	_materialsInitFunc(
		vertexId,
		rootParameters,
		staticSamplerDescs,
		rootSignatureFlag,
		rootSignatureVersion,
		shaderBuffers[0],
		shaderBuffers[1],
		shaderBuffers[2],
		shaderBuffers[3],
		shaderBuffers[4],
		graphicsPipeLineStateDesc
	);



}
