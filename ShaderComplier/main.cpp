#include <d3dcompiler.h>
#include <d3d12.h>
#include "..\CmEngine\d3dx12.h"
#include <stdint.h>
#include <fstream>
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "d3d12.lib")

using namespace std;

template<typename _charType>
void LoadStringFromFile(std::ifstream & _ifs, std::basic_string<_charType> & _dest)
{
	uint64_t stringSize = 0;
	_ifs.read(reinterpret_cast<char*>(&stringSize), sizeof(uint64_t));
	_dest.resize(stringSize);
	_ifs.read(reinterpret_cast<char*>(&_dest[0]), sizeof(std::basic_string<_charType>::value_type) * stringSize);
}

template<typename _charType>
void SaveStringToFile(std::ofstream & _ofs, const std::basic_string<_charType> & _source)
{
	uint64_t stringSize = _source.size();
	_ofs.write(reinterpret_cast<char*>(&stringSize), sizeof(uint64_t));
	_ofs.write(reinterpret_cast<const char*>(_source.data()), sizeof(std::basic_string<_charType>::value_type) * stringSize);
}

int main()
{
	CD3DX12_ROOT_PARAMETER pragmas[4];
	pragmas[0].InitAsConstantBufferView(0);
	pragmas[1].InitAsConstantBufferView(1);
	pragmas[2].InitAsDescriptorTable(1, &CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0), D3D12_SHADER_VISIBILITY_PIXEL);
	pragmas[3].InitAsDescriptorTable(1, &CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1), D3D12_SHADER_VISIBILITY_PIXEL);
	ID3DBlob * errorBlob = nullptr;
	ID3DBlob * destBlob = nullptr;

	D3D12SerializeRootSignature(&CD3DX12_ROOT_SIGNATURE_DESC(4, pragmas, 1, &CD3DX12_STATIC_SAMPLER_DESC(0), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), D3D_ROOT_SIGNATURE_VERSION_1_0, &destBlob, &errorBlob);

	uint32_t flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	ID3DBlob * vs = nullptr, *vserror = nullptr;
	HRESULT hr = D3DCompileFromFile(L"a.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSMain", "vs_5_0", flag, 0, &vs, &vserror);

	ID3DBlob * ps = nullptr, *pserror = nullptr;
	D3DCompileFromFile(L"a.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PSMain", "ps_5_0", flag, 0, &ps, &pserror);

	if (errorBlob != nullptr)
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());

	if (vserror != nullptr)
		OutputDebugStringA((char*)vserror->GetBufferPointer());
	if (pserror != nullptr)
		OutputDebugStringA((char*)pserror->GetBufferPointer());

	ofstream fs(L"default.materials", std::ios::out | std::ios::binary);

	uint64_t fitVertexIdCount = 1;
	uint64_t vertexId = 1;
	fs.write(reinterpret_cast<const char*>(&fitVertexIdCount), sizeof(uint64_t));
	fs.write(reinterpret_cast<const char*>(&vertexId), sizeof(uint64_t));

	uint64_t rootSignatureBufferSize = destBlob->GetBufferSize();
	fs.write(reinterpret_cast<const char*>(&rootSignatureBufferSize), sizeof(uint64_t));
	fs.write(reinterpret_cast<const char*>(destBlob->GetBufferPointer()), rootSignatureBufferSize);

	uint64_t shadersize = 0;

	uint64_t vssize = vs->GetBufferSize();
	fs.write(reinterpret_cast<const char*>(&vssize), sizeof(uint64_t));
	fs.write(reinterpret_cast<const char*>(vs->GetBufferPointer()), vssize);

	uint64_t pssize = ps->GetBufferSize();
	fs.write(reinterpret_cast<const char*>(&pssize), sizeof(uint64_t));
	fs.write(reinterpret_cast<const char*>(ps->GetBufferPointer()), pssize);

	fs.write(reinterpret_cast<const char*>(&shadersize), sizeof(uint64_t));
	fs.write(reinterpret_cast<const char*>(&shadersize), sizeof(uint64_t));
	fs.write(reinterpret_cast<const char*>(&shadersize), sizeof(uint64_t));

	//struct Vertex_default
	//{
	//	static constexpr uint64_t VertexId = 1;

	//	DirectX::XMFLOAT3 mPosition;
	//	DirectX::XMFLOAT2 mUV;
	//	DirectX::XMFLOAT3 mTangent;
	//	DirectX::XMFLOAT3 mBinomal;
	//};


	static constexpr D3D12_INPUT_ELEMENT_DESC InputElementDescs[] = {
		D3D12_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		D3D12_INPUT_ELEMENT_DESC{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		D3D12_INPUT_ELEMENT_DESC{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		D3D12_INPUT_ELEMENT_DESC{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc;
	ZeroMemory(&desc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	desc.SampleMask = UINT_MAX;
	desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	desc.InputLayout = D3D12_INPUT_LAYOUT_DESC{ InputElementDescs, _countof(InputElementDescs) };
	desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.NumRenderTargets = 1;
	desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.NodeMask = 0;
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	fs.write(reinterpret_cast<const char*>(&desc.InputLayout.NumElements), sizeof(UINT));
	for (int i = 0; i != 4; ++i)
	{
		//LPCSTR SemanticName;
		//UINT SemanticIndex;
		//DXGI_FORMAT Format;
		//UINT InputSlot;
		//UINT AlignedByteOffset;
		//D3D12_INPUT_CLASSIFICATION InputSlotClass;
		//UINT InstanceDataStepRate;
		auto & inputElement = desc.InputLayout.pInputElementDescs[i];
		SaveStringToFile<char>(fs, inputElement.SemanticName);
		fs.write(reinterpret_cast<const char*>(&inputElement.SemanticIndex), sizeof(UINT));
		fs.write(reinterpret_cast<const char*>(&inputElement.Format), sizeof(DXGI_FORMAT));
		fs.write(reinterpret_cast<const char*>(&inputElement.InputSlot), sizeof(UINT));
		fs.write(reinterpret_cast<const char*>(&inputElement.AlignedByteOffset), sizeof(UINT));
		fs.write(reinterpret_cast<const char*>(&inputElement.InputSlotClass), sizeof(D3D12_INPUT_CLASSIFICATION));
		fs.write(reinterpret_cast<const char*>(&inputElement.InstanceDataStepRate), sizeof(UINT));
	}

	fs.write(reinterpret_cast<const char*>(&desc.StreamOutput), sizeof(D3D12_STREAM_OUTPUT_DESC));
	fs.write(reinterpret_cast<const char*>(&desc.BlendState), sizeof(D3D12_BLEND_DESC));
	fs.write(reinterpret_cast<const char*>(&desc.SampleMask), sizeof(UINT));
	fs.write(reinterpret_cast<const char*>(&desc.RasterizerState), sizeof(D3D12_RASTERIZER_DESC));
	fs.write(reinterpret_cast<const char*>(&desc.DepthStencilState), sizeof(D3D12_DEPTH_STENCIL_DESC));

	fs.write(reinterpret_cast<const char*>(&desc.IBStripCutValue), sizeof(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE));
	fs.write(reinterpret_cast<const char*>(&desc.PrimitiveTopologyType), sizeof(D3D12_PRIMITIVE_TOPOLOGY_TYPE));
	fs.write(reinterpret_cast<const char*>(&desc.NumRenderTargets), sizeof(UINT));
	fs.write(reinterpret_cast<const char*>(desc.RTVFormats), sizeof(DXGI_FORMAT) * _countof(desc.RTVFormats));
	fs.write(reinterpret_cast<const char*>(&desc.DSVFormat), sizeof(DXGI_FORMAT));
	fs.write(reinterpret_cast<const char*>(&desc.SampleDesc), sizeof(DXGI_SAMPLE_DESC));
	fs.write(reinterpret_cast<const char*>(&desc.NodeMask), sizeof(UINT));
	fs.write(reinterpret_cast<const char*>(&desc.CachedPSO), sizeof(D3D12_CACHED_PIPELINE_STATE));
	fs.write(reinterpret_cast<const char*>(&desc.Flags), sizeof(D3D12_PIPELINE_STATE_FLAGS));
	fs.close();

	fs.open(L"MaterialsManager.data", ios_base::out | std::ios::binary);

	uint64_t a = 1;
	fs.write((const char*)&a, sizeof(uint64_t));
	SaveStringToFile<wchar_t>(fs, L"defaultmaterials");
	SaveStringToFile<wchar_t>(fs, L"default.materials");
	fs.close();
}
