#include "StaticMeshActor.h"

void AStaticMeshActor::SetMaterials(UMaterials * _materials)
{
	Assert(_materials->IsFittedVertexId(mStaticMesh->GetVertexId()));


	const std::vector<D3D12_ROOT_PARAMETER_TYPE> & newSpi = _materials->GetParameterIdentifications();
	mShaderParameters.resize(newSpi.size());

	for (uint64_t i = 0; i != newSpi.size(); ++i)
	{
		mShaderParameters[i].mParameterType = newSpi[i];
	}

	mMaterials = _materials;
}
