#include "Materials.h"
#include "d3dx12.h"
#include <fstream>
#include <string>
#include <vector>
#include "D3D12RhiGlobal.h"
#include "D3D12RhiUtil.h"

void UMaterials::Init(std::wstring _fileName)
{
	std::ifstream fs(_fileName, std::ios::binary | std::ios::in);

	uint64_t fitVertexIdCount = 0;
	fs.read(reinterpret_cast<char*>(&fitVertexIdCount), sizeof(uint64_t));
	for (int i = 0; i != fitVertexIdCount; ++i)
	{
		uint64_t fitVertexId = 0;
		fs.read(reinterpret_cast<char*>(&fitVertexId), sizeof(uint64_t));
		mFitVertexId.insert(fitVertexId);
	}

	uint64_t rootSignatureBufferSize = 0;
	void * rootSignatureBuffer = nullptr;
	fs.read(reinterpret_cast<char*>(&rootSignatureBufferSize), sizeof(uint64_t));
	rootSignatureBuffer = malloc(rootSignatureBufferSize);
	fs.read(reinterpret_cast<char*>(rootSignatureBuffer), rootSignatureBufferSize);
	VerifyD3D12Result(GDevice->CreateRootSignature(0, rootSignatureBuffer, rootSignatureBufferSize, IID_PPV_ARGS(&mRootSignature)));
	free(rootSignatureBuffer);

	uint64_t shaderBufferSize[5] = {};
	void * shaderBuffer[5] = {};

	for (int i = 0; i != 5; ++i)
	{
		fs.read(reinterpret_cast<char*>(&shaderBufferSize[i]), sizeof(uint64_t));
		if (shaderBufferSize[i] != 0)
		{
			shaderBuffer[i] = malloc(shaderBufferSize[i]);
			fs.read(reinterpret_cast<char*>(shaderBuffer[i]), shaderBufferSize[i]);
		}
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc;

	desc.pRootSignature = mRootSignature;
	desc.VS = { shaderBuffer[0], shaderBufferSize[0] };
	desc.PS = { shaderBuffer[1], shaderBufferSize[1] };
	desc.DS = { shaderBuffer[2], shaderBufferSize[2] };
	desc.HS = { shaderBuffer[3], shaderBufferSize[3] };
	desc.GS = { shaderBuffer[4], shaderBufferSize[4] };

	UINT numInputElements = 0;
	D3D12_INPUT_ELEMENT_DESC * inputElements = nullptr;
	std::string *SemanticName = nullptr;
	fs.read(reinterpret_cast<char*>(&numInputElements), sizeof(UINT));
	if (numInputElements != 0)
	{
		inputElements = new D3D12_INPUT_ELEMENT_DESC[numInputElements];
		SemanticName = new std::string[numInputElements];
		for (UINT i = 0; i != numInputElements; ++i)
		{
			LoadStringFromFile(fs, SemanticName[i]);
			inputElements[i].SemanticName = SemanticName[i].c_str();
			fs.read(reinterpret_cast<char*>(&inputElements[i].SemanticIndex), sizeof(UINT));
			fs.read(reinterpret_cast<char*>(&inputElements[i].Format), sizeof(DXGI_FORMAT));
			fs.read(reinterpret_cast<char*>(&inputElements[i].InputSlot), sizeof(UINT));
			fs.read(reinterpret_cast<char*>(&inputElements[i].AlignedByteOffset), sizeof(UINT));
			fs.read(reinterpret_cast<char*>(&inputElements[i].InputSlotClass), sizeof(D3D12_INPUT_CLASSIFICATION));
			fs.read(reinterpret_cast<char*>(&inputElements[i].InstanceDataStepRate), sizeof(UINT));
		}
	}

	desc.InputLayout.NumElements = numInputElements;
	desc.InputLayout.pInputElementDescs = inputElements;

	fs.read(reinterpret_cast<char*>(&desc.StreamOutput), sizeof(D3D12_STREAM_OUTPUT_DESC));
	fs.read(reinterpret_cast<char*>(&desc.BlendState), sizeof(D3D12_BLEND_DESC));
	fs.read(reinterpret_cast<char*>(&desc.SampleMask), sizeof(UINT));
	fs.read(reinterpret_cast<char*>(&desc.RasterizerState), sizeof(D3D12_RASTERIZER_DESC));
	fs.read(reinterpret_cast<char*>(&desc.DepthStencilState), sizeof(D3D12_DEPTH_STENCIL_DESC));

	fs.read(reinterpret_cast<char*>(&desc.IBStripCutValue), sizeof(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE));
	fs.read(reinterpret_cast<char*>(&desc.PrimitiveTopologyType), sizeof(D3D12_PRIMITIVE_TOPOLOGY_TYPE));
	fs.read(reinterpret_cast<char*>(&desc.NumRenderTargets), sizeof(UINT));
	fs.read(reinterpret_cast<char*>(desc.RTVFormats), sizeof(DXGI_FORMAT) * _countof(desc.RTVFormats));
	fs.read(reinterpret_cast<char*>(&desc.DSVFormat), sizeof(DXGI_FORMAT));
	fs.read(reinterpret_cast<char*>(&desc.SampleDesc), sizeof(DXGI_SAMPLE_DESC));
	fs.read(reinterpret_cast<char*>(&desc.NodeMask), sizeof(UINT));
	fs.read(reinterpret_cast<char*>(&desc.CachedPSO), sizeof(D3D12_CACHED_PIPELINE_STATE));
	fs.read(reinterpret_cast<char*>(&desc.Flags), sizeof(D3D12_PIPELINE_STATE_FLAGS));

	VerifyD3D12Result(GDevice->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&mPipelineState)));

	for (int i = 0; i != 5; ++i)
		if (shaderBuffer[i] != nullptr)
			free(shaderBuffer[i]);

	if (inputElements != nullptr)
		delete inputElements;

	fs.close();
}

void UMaterials::Clear()
{
	mRootSignature->Release();
	mPipelineState->Release();

	mRootSignature = nullptr;
	mPipelineState = nullptr;
}
