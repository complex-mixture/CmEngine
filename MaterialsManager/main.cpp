#include "MaterialsManager.h"
#include <d3dcompiler.h>

//_Out_ Comment(VertexId) std::vector<uint64_t> & _suitedVertexIds, \
//_Out_ std::vector<D3D12_ROOT_PARAMETER_TYPE> & _parameterIdentifications, \
//_Out_ std::vector<CD3DX12_ROOT_PARAMETER> & _rootParameters, \
//_Out_ std::vector<CD3DX12_STATIC_SAMPLER_DESC> & _staticSamplerDescs, \
//_Out_ D3D12_ROOT_SIGNATURE_FLAGS & _rootSignatureFlag, \
//_Out_ D3D_ROOT_SIGNATURE_VERSION & _rootSignatureVersion, \
//_Out_ Comment(VS) ID3DBlob *& _vs, \
//_Out_ Comment(HS) ID3DBlob *& _hs, \
//_Out_ Comment(DS) ID3DBlob *& _ds, \
//_Out_ Comment(GS) ID3DBlob *& _gs, \
//_Out_ Comment(PS) ID3DBlob *& _ps, \
//_Out_ D3D12_GRAPHICS_PIPELINE_STATE_DESC & _graphicsPipeLineStateDesc\

int main()
{
	bool IsDebug = false;
	UINT CompilerFlag1 = 0;
	if (IsDebug)
		CompilerFlag1 |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;


	FMaterialsManager::Get().Init();

	FMaterialsManager::Get().AddMaterials(
		MaterialsInitFuncLeader(&) {

		_suitedVertexIds.resize(1);
		_suitedVertexIds[0] = 0;

		_parameterIdentifications.resize(1);
		_parameterIdentifications[0] = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;

		_rootParameters.resize(3);
		_rootParameters[0].InitAsConstantBufferView(0);
		_rootParameters[1].InitAsConstantBufferView(1);
		_rootParameters[2].InitAsDescriptorTable(1, &CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0), D3D12_SHADER_VISIBILITY_PIXEL);

		_staticSamplerDescs.resize(1);
		_staticSamplerDescs[0].Init(0);

		_rootSignatureFlag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		_rootSignatureVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;

		ID3DBlob * vsError = nullptr;
		D3DCompileFromFile(L"BaseColor.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VsMain", "vs_5_0", CompilerFlag1, 0, &_vs, &vsError);

		ID3DBlob * psError = nullptr;
		D3DCompileFromFile(L"BaseColor.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PsMain", "ps_5_0", CompilerFlag1, 0, &_ps, &psError);

		if (vsError)
			DebugMessageBoxA("Vs Compile Error", reinterpret_cast<char*>(vsError->GetBufferPointer()));

		if (psError)
			DebugMessageBoxA("Ps Compile Error", reinterpret_cast<char*>(psError->GetBufferPointer()));

		static D3D12_INPUT_ELEMENT_DESC InputElementDescs[] = {
			D3D12_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			D3D12_INPUT_ELEMENT_DESC{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		ZeroMemory(&_graphicsPipeLineStateDesc, sizeof(_graphicsPipeLineStateDesc));
		_graphicsPipeLineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.SampleMask = UINT_MAX;
		_graphicsPipeLineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.InputLayout = D3D12_INPUT_LAYOUT_DESC{ InputElementDescs, _countof(InputElementDescs) };
		_graphicsPipeLineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		_graphicsPipeLineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		_graphicsPipeLineStateDesc.NumRenderTargets = 1;
		_graphicsPipeLineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		_graphicsPipeLineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		_graphicsPipeLineStateDesc.SampleDesc.Count = 1;
		_graphicsPipeLineStateDesc.SampleDesc.Quality = 0;
		_graphicsPipeLineStateDesc.NodeMask = 0;
		_graphicsPipeLineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	},
		L"BaseColor",
		L"BaseColor.materials"
		);

	FMaterialsManager::Get().AddMaterials(
		MaterialsInitFuncLeader(&) {

		_suitedVertexIds.resize(1);
		_suitedVertexIds[0] = 0;

		_parameterIdentifications.resize(1);
		_parameterIdentifications[0] = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;

		_rootParameters.resize(3);
		_rootParameters[0].InitAsConstantBufferView(0);
		_rootParameters[1].InitAsConstantBufferView(1);
		_rootParameters[2].InitAsDescriptorTable(1, &CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0), D3D12_SHADER_VISIBILITY_PIXEL);

		_staticSamplerDescs.resize(1);
		_staticSamplerDescs[0].Init(0);

		_rootSignatureFlag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		_rootSignatureVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;

		ID3DBlob * vsError = nullptr;
		D3DCompileFromFile(L"WorldNormal.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VsMain", "vs_5_0", CompilerFlag1, 0, &_vs, &vsError);

		ID3DBlob * psError = nullptr;
		D3DCompileFromFile(L"WorldNormal.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PsMain", "ps_5_0", CompilerFlag1, 0, &_ps, &psError);

		if (vsError)
			DebugMessageBoxA("Vs Compile Error", reinterpret_cast<char*>(vsError->GetBufferPointer()));

		if (psError)
			DebugMessageBoxA("Ps Compile Error", reinterpret_cast<char*>(psError->GetBufferPointer()));

		static D3D12_INPUT_ELEMENT_DESC InputElementDescs[] = {
			D3D12_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			D3D12_INPUT_ELEMENT_DESC{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			D3D12_INPUT_ELEMENT_DESC{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			D3D12_INPUT_ELEMENT_DESC{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		ZeroMemory(&_graphicsPipeLineStateDesc, sizeof(_graphicsPipeLineStateDesc));
		_graphicsPipeLineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.SampleMask = UINT_MAX;
		_graphicsPipeLineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.InputLayout = D3D12_INPUT_LAYOUT_DESC{ InputElementDescs, _countof(InputElementDescs) };
		_graphicsPipeLineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		_graphicsPipeLineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		_graphicsPipeLineStateDesc.NumRenderTargets = 1;
		_graphicsPipeLineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		_graphicsPipeLineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		_graphicsPipeLineStateDesc.SampleDesc.Count = 1;
		_graphicsPipeLineStateDesc.SampleDesc.Quality = 0;
		_graphicsPipeLineStateDesc.NodeMask = 0;
		_graphicsPipeLineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	},
		L"WorldNormal",
		L"WorldNormal.materials"
		);

	FMaterialsManager::Get().AddMaterials(
		MaterialsInitFuncLeader(&) {

		_suitedVertexIds.resize(1);
		_suitedVertexIds[0] = 0;

		_rootParameters.resize(2);
		_rootParameters[0].InitAsConstantBufferView(0);
		_rootParameters[1].InitAsConstantBufferView(1);

		_rootSignatureFlag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		_rootSignatureVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;

		ID3DBlob * vsError = nullptr;
		D3DCompileFromFile(L"DebugUv.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VsMain", "vs_5_0", CompilerFlag1, 0, &_vs, &vsError);

		ID3DBlob * psError = nullptr;
		D3DCompileFromFile(L"DebugUv.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PsMain", "ps_5_0", CompilerFlag1, 0, &_ps, &psError);

		if (vsError)
			DebugMessageBoxA("Vs Compile Error", reinterpret_cast<char*>(vsError->GetBufferPointer()));

		if (psError)
			DebugMessageBoxA("Ps Compile Error", reinterpret_cast<char*>(psError->GetBufferPointer()));

		static D3D12_INPUT_ELEMENT_DESC InputElementDescs[] = {
			D3D12_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			D3D12_INPUT_ELEMENT_DESC{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		ZeroMemory(&_graphicsPipeLineStateDesc, sizeof(_graphicsPipeLineStateDesc));
		_graphicsPipeLineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.SampleMask = UINT_MAX;
		_graphicsPipeLineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.InputLayout = D3D12_INPUT_LAYOUT_DESC{ InputElementDescs, _countof(InputElementDescs) };
		_graphicsPipeLineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		_graphicsPipeLineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		_graphicsPipeLineStateDesc.NumRenderTargets = 1;
		_graphicsPipeLineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		_graphicsPipeLineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		_graphicsPipeLineStateDesc.SampleDesc.Count = 1;
		_graphicsPipeLineStateDesc.SampleDesc.Quality = 0;
		_graphicsPipeLineStateDesc.NodeMask = 0;
		_graphicsPipeLineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	},
		L"DebugUv",
		L"DebugUv.materials"
		);

	FMaterialsManager::Get().AddMaterials(
		MaterialsInitFuncLeader(&) {

		_suitedVertexIds.resize(1);
		_suitedVertexIds[0] = 0;

		_parameterIdentifications.resize(2);
		_parameterIdentifications[0] = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		_parameterIdentifications[1] = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;

		_rootParameters.resize(4);
		_rootParameters[0].InitAsConstantBufferView(0);
		_rootParameters[1].InitAsConstantBufferView(1);
		_rootParameters[2].InitAsDescriptorTable(1, &CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0), D3D12_SHADER_VISIBILITY_PIXEL);
		_rootParameters[3].InitAsDescriptorTable(1, &CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1), D3D12_SHADER_VISIBILITY_PIXEL);

		_staticSamplerDescs.resize(1);
		_staticSamplerDescs[0].Init(0);

		_rootSignatureFlag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		_rootSignatureVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;

		ID3DBlob * vsError = nullptr;
		D3DCompileFromFile(L"Gooch.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VsMain", "vs_5_0", CompilerFlag1, 0, &_vs, &vsError);

		ID3DBlob * psError = nullptr;
		D3DCompileFromFile(L"Gooch.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PsMain", "ps_5_0", CompilerFlag1, 0, &_ps, &psError);

		if (vsError)
			DebugMessageBoxA("Vs Compile Error", reinterpret_cast<char*>(vsError->GetBufferPointer()));

		if (psError)
			DebugMessageBoxA("Ps Compile Error", reinterpret_cast<char*>(psError->GetBufferPointer()));

		static D3D12_INPUT_ELEMENT_DESC InputElementDescs[] = {
			D3D12_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			D3D12_INPUT_ELEMENT_DESC{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			D3D12_INPUT_ELEMENT_DESC{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			D3D12_INPUT_ELEMENT_DESC{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		ZeroMemory(&_graphicsPipeLineStateDesc, sizeof(_graphicsPipeLineStateDesc));
		_graphicsPipeLineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.SampleMask = UINT_MAX;
		_graphicsPipeLineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.InputLayout = D3D12_INPUT_LAYOUT_DESC{ InputElementDescs, _countof(InputElementDescs) };
		_graphicsPipeLineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		_graphicsPipeLineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		_graphicsPipeLineStateDesc.NumRenderTargets = 1;
		_graphicsPipeLineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		_graphicsPipeLineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		_graphicsPipeLineStateDesc.SampleDesc.Count = 1;
		_graphicsPipeLineStateDesc.SampleDesc.Quality = 0;
		_graphicsPipeLineStateDesc.NodeMask = 0;
		_graphicsPipeLineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	},
		L"Gooch",
		L"Gooch.materials"
		);

	FMaterialsManager::Get().AddMaterials(
		MaterialsInitFuncLeader(&) {

		_suitedVertexIds.resize(1);
		_suitedVertexIds[0] = 0;

		_parameterIdentifications.resize(2);
		_parameterIdentifications[0] = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		_parameterIdentifications[1] = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;

		_rootParameters.resize(4);
		_rootParameters[0].InitAsConstantBufferView(0);
		_rootParameters[1].InitAsConstantBufferView(1);
		_rootParameters[2].InitAsDescriptorTable(1, &CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0), D3D12_SHADER_VISIBILITY_PIXEL);
		_rootParameters[3].InitAsDescriptorTable(1, &CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1), D3D12_SHADER_VISIBILITY_PIXEL);

		_staticSamplerDescs.resize(1);
		_staticSamplerDescs[0].Init(0);

		_rootSignatureFlag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		_rootSignatureVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;

		ID3DBlob * vsError = nullptr;
		D3DCompileFromFile(L"GoochChair.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VsMain", "vs_5_0", CompilerFlag1, 0, &_vs, &vsError);

		ID3DBlob * psError = nullptr;
		D3DCompileFromFile(L"GoochChair.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PsMain", "ps_5_0", CompilerFlag1, 0, &_ps, &psError);

		if (vsError)
			DebugMessageBoxA("Vs Compile Error", reinterpret_cast<char*>(vsError->GetBufferPointer()));

		if (psError)
			DebugMessageBoxA("Ps Compile Error", reinterpret_cast<char*>(psError->GetBufferPointer()));

		static D3D12_INPUT_ELEMENT_DESC InputElementDescs[] = {
			D3D12_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			D3D12_INPUT_ELEMENT_DESC{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			D3D12_INPUT_ELEMENT_DESC{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			D3D12_INPUT_ELEMENT_DESC{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		ZeroMemory(&_graphicsPipeLineStateDesc, sizeof(_graphicsPipeLineStateDesc));
		_graphicsPipeLineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.SampleMask = UINT_MAX;
		_graphicsPipeLineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.InputLayout = D3D12_INPUT_LAYOUT_DESC{ InputElementDescs, _countof(InputElementDescs) };
		_graphicsPipeLineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		_graphicsPipeLineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		_graphicsPipeLineStateDesc.NumRenderTargets = 1;
		_graphicsPipeLineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		_graphicsPipeLineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		_graphicsPipeLineStateDesc.SampleDesc.Count = 1;
		_graphicsPipeLineStateDesc.SampleDesc.Quality = 0;
		_graphicsPipeLineStateDesc.NodeMask = 0;
		_graphicsPipeLineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	},
		L"GoochChair",
		L"GoochChair.materials"
		);

	FMaterialsManager::Get().AddMaterials(
		MaterialsInitFuncLeader(&) {

		_suitedVertexIds.resize(1);
		_suitedVertexIds[0] = 1;

		_parameterIdentifications.resize(1);
		_parameterIdentifications[0] = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;

		_rootParameters.resize(3);
		_rootParameters[0].InitAsConstantBufferView(0);
		_rootParameters[1].InitAsConstantBufferView(1);
		_rootParameters[2].InitAsDescriptorTable(1, &CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0), D3D12_SHADER_VISIBILITY_PIXEL);

		_staticSamplerDescs.resize(1);
		_staticSamplerDescs[0].Init(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_MIRROR, D3D12_TEXTURE_ADDRESS_MODE_MIRROR, D3D12_TEXTURE_ADDRESS_MODE_MIRROR);

		_rootSignatureFlag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		_rootSignatureVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;

		ID3DBlob * vsError = nullptr;
		D3DCompileFromFile(L"SkyBox.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VsMain", "vs_5_0", CompilerFlag1, 0, &_vs, &vsError);

		ID3DBlob * psError = nullptr;
		D3DCompileFromFile(L"SkyBox.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PsMain", "ps_5_0", CompilerFlag1, 0, &_ps, &psError);

		if (vsError)
			DebugMessageBoxA("Vs Compile Error", reinterpret_cast<char*>(vsError->GetBufferPointer()));

		if (psError)
			DebugMessageBoxA("Ps Compile Error", reinterpret_cast<char*>(psError->GetBufferPointer()));

		static D3D12_INPUT_ELEMENT_DESC InputElementDescs[] = {
			D3D12_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		ZeroMemory(&_graphicsPipeLineStateDesc, sizeof(_graphicsPipeLineStateDesc));
		_graphicsPipeLineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.SampleMask = UINT_MAX;
		_graphicsPipeLineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
		_graphicsPipeLineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		_graphicsPipeLineStateDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

		_graphicsPipeLineStateDesc.InputLayout = D3D12_INPUT_LAYOUT_DESC{ InputElementDescs, _countof(InputElementDescs) };
		_graphicsPipeLineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		_graphicsPipeLineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		_graphicsPipeLineStateDesc.NumRenderTargets = 1;
		_graphicsPipeLineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		_graphicsPipeLineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		_graphicsPipeLineStateDesc.SampleDesc.Count = 1;
		_graphicsPipeLineStateDesc.SampleDesc.Quality = 0;
		_graphicsPipeLineStateDesc.NodeMask = 0;
		_graphicsPipeLineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	},
		L"SkyBox",
		L"SkyBox.materials"
		);

	FMaterialsManager::Get().AddMaterials(
		MaterialsInitFuncLeader(&) {

		_suitedVertexIds.resize(1);
		_suitedVertexIds[0] = 0;

		_parameterIdentifications.resize(2);
		_parameterIdentifications[0] = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		_parameterIdentifications[1] = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;

		_rootParameters.resize(4);
		_rootParameters[0].InitAsConstantBufferView(0);
		_rootParameters[1].InitAsConstantBufferView(1);
		_rootParameters[2].InitAsDescriptorTable(1, &CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0), D3D12_SHADER_VISIBILITY_PIXEL);
		_rootParameters[3].InitAsDescriptorTable(1, &CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1), D3D12_SHADER_VISIBILITY_PIXEL);

		_staticSamplerDescs.resize(1);
		_staticSamplerDescs[0].Init(0);

		_rootSignatureFlag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		_rootSignatureVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;

		ID3DBlob * vsError = nullptr;
		D3DCompileFromFile(L"Pi.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VsMain", "vs_5_0", CompilerFlag1, 0, &_vs, &vsError);

		ID3DBlob * psError = nullptr;
		D3DCompileFromFile(L"Pi.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PsMain", "ps_5_0", CompilerFlag1, 0, &_ps, &psError);

		if (vsError)
			DebugMessageBoxA("Vs Compile Error", reinterpret_cast<char*>(vsError->GetBufferPointer()));

		if (psError)
			DebugMessageBoxA("Ps Compile Error", reinterpret_cast<char*>(psError->GetBufferPointer()));

		static D3D12_INPUT_ELEMENT_DESC InputElementDescs[] = {
			D3D12_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			D3D12_INPUT_ELEMENT_DESC{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			D3D12_INPUT_ELEMENT_DESC{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			D3D12_INPUT_ELEMENT_DESC{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		ZeroMemory(&_graphicsPipeLineStateDesc, sizeof(_graphicsPipeLineStateDesc));
		_graphicsPipeLineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.SampleMask = UINT_MAX;
		_graphicsPipeLineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		_graphicsPipeLineStateDesc.InputLayout = D3D12_INPUT_LAYOUT_DESC{ InputElementDescs, _countof(InputElementDescs) };
		_graphicsPipeLineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		_graphicsPipeLineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		_graphicsPipeLineStateDesc.NumRenderTargets = 1;
		_graphicsPipeLineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		_graphicsPipeLineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		_graphicsPipeLineStateDesc.SampleDesc.Count = 1;
		_graphicsPipeLineStateDesc.SampleDesc.Quality = 0;
		_graphicsPipeLineStateDesc.NodeMask = 0;
		_graphicsPipeLineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	},
		L"Pi",
		L"Pi.materials"
		);

	FMaterialsManager::Get().Clear();
}
