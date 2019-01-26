#pragma once
#include "Actor.h"
#include "Mesh.h"
#include "Materials.h"
#include "Util.h"
#include "ShaderParameter.h"
#include "EEntityType.h"

class AStaticMeshActor : public AActor
{
	SpecificEntityType(EEntityType::SaticMesh)

public:
	void SetStaticMesh(UMesh * _staticMesh) { mStaticMesh = _staticMesh; }
	//after reset materials, the shader parameters will be reset
	void SetMaterials(UMaterials * _materials);
	UMesh * GetStaticMesh()const { return mStaticMesh; }
	UMaterials * GetMaterials()const { return mMaterials; }
	const std::vector<FShaderParameter> & GetShaderParameters()const { return mShaderParameters; }

	void SetMaterialsParameterDescriptorTable(uint64_t _slot, D3D12_GPU_DESCRIPTOR_HANDLE _parameter)
	{
		Assert(mShaderParameters.size() > _slot);
		Assert(mShaderParameters[_slot].mParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE);
		mShaderParameters[_slot].mParameterDescriptorTable = _parameter;
	}

	void SetMaterialsParameterConstantBufferView(uint64_t _slot, D3D12_GPU_VIRTUAL_ADDRESS _parameter)
	{
		Assert(mShaderParameters.size() > _slot);
		Assert(mShaderParameters[_slot].mParameterType == D3D12_ROOT_PARAMETER_TYPE_CBV);
		mShaderParameters[_slot].mParameterConstantBufferView = _parameter;
	}

	void SetMaterialsParameterShaderResourceView(uint64_t _slot, D3D12_GPU_VIRTUAL_ADDRESS _parameter)
	{
		Assert(mShaderParameters.size() > _slot);
		Assert(mShaderParameters[_slot].mParameterType == D3D12_ROOT_PARAMETER_TYPE_SRV);
		mShaderParameters[_slot].mParameterShaderResourcedView = _parameter;
	}

	void SetMaterialsParameterUnorderedAccessView(uint64_t _slot, D3D12_GPU_VIRTUAL_ADDRESS _parameter)
	{
		Assert(mShaderParameters.size() > _slot);
		Assert(mShaderParameters[_slot].mParameterType == D3D12_ROOT_PARAMETER_TYPE_UAV);
		mShaderParameters[_slot].mParameterUnorderedAccessView = _parameter;
	}

	void EndPlay()override {}

private:
	UMesh * mStaticMesh;
	UMaterials * mMaterials;
	std::vector<FShaderParameter> mShaderParameters;
};
