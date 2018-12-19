#pragma once
#include "Actor.h"
#include "StaticMesh.h"
#include "Materials.h"
#include "Util.h"

class AStaticMeshActor : public AActor
{
public:
	void SetStaticMesh(UStaticMesh * _staticMesh) { mStaticMesh = _staticMesh; }
	void SetMaterials(UMaterials * _materials) { Assert(_materials->IsFittedVertexId(mStaticMesh->GetVertexId())); mMaterials = _materials; }
	UStaticMesh * GetStaticMesh()const { return mStaticMesh; }
	UMaterials * GetMaterials()const { return mMaterials; }
private:
	UStaticMesh * mStaticMesh;
	UMaterials * mMaterials;
};