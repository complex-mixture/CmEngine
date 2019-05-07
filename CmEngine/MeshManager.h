#pragma once
#include "Util.h"
#include <map>
#include "ResourceManagerBase.h"
#include "Template.h"

class UMesh;
class FMeshManager final : public FResourceManagerBase<UMesh>, public Tpl::TAsSingle<FMeshManager>
{
public:
	void Init();
	void Clear();

private:
	std::wstring mFileName = L"MeshManager.data";
};
