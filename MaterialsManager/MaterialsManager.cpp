#include "MaterialsManager.h"
#include <fstream>
#include "Util.h"

#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "d3d12.lib")

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

void FMaterialsManager::Save()
{
	std::ofstream ofs(mManagerFileName, std::ios::out | std::ios::binary);
	uint64_t elementCount = mMap.size();
	ofs.write(reinterpret_cast<const char*>(&elementCount), sizeof(uint64_t));
	for (auto const & _ele : mMap)
	{
		SaveStringToFile<wchar_t>(ofs, _ele.first);
		SaveStringToFile<wchar_t>(ofs, _ele.second);
	}
}

void FMaterialsManager::Clear()
{
	Save();
}

void FMaterialsManager::AddMaterials(
	std::function<void(
		_Out_ Comment(VertexIds) std::vector<uint64_t> &,
		_Out_ Comment(SuitedEntityType) std::vector<EEntityType> &,
		_Out_ Comment(parameterIdentifications) std::vector<D3D12_ROOT_PARAMETER_TYPE> &,
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
	std::vector<uint64_t> suitedVertexIds;
	std::vector<EEntityType> suitedEntityType;
	std::vector<D3D12_ROOT_PARAMETER_TYPE> parameterIdentifications;
	std::vector<CD3DX12_ROOT_PARAMETER> rootParameters;
	std::vector<CD3DX12_STATIC_SAMPLER_DESC> staticSamplerDescs;
	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlag = D3D12_ROOT_SIGNATURE_FLAG_NONE;
	D3D_ROOT_SIGNATURE_VERSION rootSignatureVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
	ID3DBlob * shaderBlobs[5] = {};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipeLineStateDesc;

	_materialsInitFunc(
		suitedVertexIds,
		suitedEntityType,
		parameterIdentifications,
		rootParameters,
		staticSamplerDescs,
		rootSignatureFlag,
		rootSignatureVersion,
		shaderBlobs[0],
		shaderBlobs[1],
		shaderBlobs[2],
		shaderBlobs[3],
		shaderBlobs[4],
		graphicsPipeLineStateDesc
	);

	ID3DBlob * rootSignatureBlob = nullptr;
	ID3DBlob * rootSignatureErrorBlob = nullptr;

	HRESULT hr =  D3D12SerializeRootSignature(
		&CD3DX12_ROOT_SIGNATURE_DESC(
			rootParameters.size(),
			rootParameters.size() != 0 ? &rootParameters[0] : nullptr,
			staticSamplerDescs.size(),
			staticSamplerDescs.size() != 0 ? &staticSamplerDescs[0] : nullptr,
			rootSignatureFlag
		),
		rootSignatureVersion,
		&rootSignatureBlob,
		&rootSignatureErrorBlob
	);

	if (rootSignatureErrorBlob)
	{
		DebugMessageBoxA("D3D12SerializeRootSignature Error", reinterpret_cast<const char*>(rootSignatureErrorBlob->GetBufferPointer()));
	}

	std::ofstream ofs(_destFileName, std::ios::out | std::ios::binary);

	uint64_t suitedVertexIdCount = suitedVertexIds.size();
	ofs.write(reinterpret_cast<const char*>(&suitedVertexIdCount), sizeof(uint64_t));
	ofs.write(reinterpret_cast<const char*>(suitedVertexIds.data()), sizeof(uint64_t) * suitedVertexIdCount);

	uint64_t suitedEntityTypeCount = suitedEntityType.size();
	ofs.write(reinterpret_cast<const char*>(&suitedEntityTypeCount), sizeof(uint64_t));
	ofs.write(reinterpret_cast<const char*>(suitedEntityType.data()), sizeof(EEntityType) * suitedEntityTypeCount);

	uint64_t parameterIdentificationCount = parameterIdentifications.size();
	ofs.write(reinterpret_cast<const char*>(&parameterIdentificationCount), sizeof(uint64_t));
	ofs.write(reinterpret_cast<const char*>(parameterIdentifications.data()), sizeof(D3D12_ROOT_PARAMETER_TYPE) * parameterIdentificationCount);

	uint64_t rootSignatureBufferSize = rootSignatureBlob->GetBufferSize();
	ofs.write(reinterpret_cast<const char*>(&rootSignatureBufferSize), sizeof(uint64_t));
	ofs.write(reinterpret_cast<const char*>(rootSignatureBlob->GetBufferPointer()), rootSignatureBufferSize);

	for (int i = 0; i != 5; ++i)
	{
		uint64_t shaderBlobSize = shaderBlobs[i] ? shaderBlobs[i]->GetBufferSize() : 0;
		ofs.write(reinterpret_cast<const char*>(&shaderBlobSize), sizeof(uint64_t));
		if (shaderBlobs[i] != nullptr)
		{
			ofs.write(reinterpret_cast<const char*>(shaderBlobs[i]->GetBufferPointer()), shaderBlobSize);
		}
	}

	ofs.write(reinterpret_cast<const char*>(&graphicsPipeLineStateDesc.InputLayout.NumElements), sizeof(UINT));
	for (int i = 0; i != graphicsPipeLineStateDesc.InputLayout.NumElements; ++i)
	{
		auto & inputElement = graphicsPipeLineStateDesc.InputLayout.pInputElementDescs[i];
		SaveStringToFile<char>(ofs, inputElement.SemanticName);
		ofs.write(reinterpret_cast<const char*>(&inputElement.SemanticIndex), sizeof(UINT));
		ofs.write(reinterpret_cast<const char*>(&inputElement.Format), sizeof(DXGI_FORMAT));
		ofs.write(reinterpret_cast<const char*>(&inputElement.InputSlot), sizeof(UINT));
		ofs.write(reinterpret_cast<const char*>(&inputElement.AlignedByteOffset), sizeof(UINT));
		ofs.write(reinterpret_cast<const char*>(&inputElement.InputSlotClass), sizeof(D3D12_INPUT_CLASSIFICATION));
		ofs.write(reinterpret_cast<const char*>(&inputElement.InstanceDataStepRate), sizeof(UINT));
	}

	ofs.write(reinterpret_cast<const char*>(&graphicsPipeLineStateDesc.StreamOutput), sizeof(D3D12_STREAM_OUTPUT_DESC));
	ofs.write(reinterpret_cast<const char*>(&graphicsPipeLineStateDesc.BlendState), sizeof(D3D12_BLEND_DESC));
	ofs.write(reinterpret_cast<const char*>(&graphicsPipeLineStateDesc.SampleMask), sizeof(UINT));
	ofs.write(reinterpret_cast<const char*>(&graphicsPipeLineStateDesc.RasterizerState), sizeof(D3D12_RASTERIZER_DESC));
	ofs.write(reinterpret_cast<const char*>(&graphicsPipeLineStateDesc.DepthStencilState), sizeof(D3D12_DEPTH_STENCIL_DESC));
	ofs.write(reinterpret_cast<const char*>(&graphicsPipeLineStateDesc.IBStripCutValue), sizeof(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE));
	ofs.write(reinterpret_cast<const char*>(&graphicsPipeLineStateDesc.PrimitiveTopologyType), sizeof(D3D12_PRIMITIVE_TOPOLOGY_TYPE));
	ofs.write(reinterpret_cast<const char*>(&graphicsPipeLineStateDesc.NumRenderTargets), sizeof(UINT));
	ofs.write(reinterpret_cast<const char*>(graphicsPipeLineStateDesc.RTVFormats), sizeof(DXGI_FORMAT) * _countof(graphicsPipeLineStateDesc.RTVFormats));
	ofs.write(reinterpret_cast<const char*>(&graphicsPipeLineStateDesc.DSVFormat), sizeof(DXGI_FORMAT));
	ofs.write(reinterpret_cast<const char*>(&graphicsPipeLineStateDesc.SampleDesc), sizeof(DXGI_SAMPLE_DESC));
	ofs.write(reinterpret_cast<const char*>(&graphicsPipeLineStateDesc.NodeMask), sizeof(UINT));
	ofs.write(reinterpret_cast<const char*>(&graphicsPipeLineStateDesc.CachedPSO), sizeof(D3D12_CACHED_PIPELINE_STATE));
	ofs.write(reinterpret_cast<const char*>(&graphicsPipeLineStateDesc.Flags), sizeof(D3D12_PIPELINE_STATE_FLAGS));
	ofs.close();

	mMap.insert(std::make_pair(_materialsName, _destFileName));
}
