#pragma once
#include "Util.h"
#include <map>
#include "ResourceManagerBase.h"
#include "Template.h"
class UMaterials;

class FMaterialsManager final : public FResourceManagerBase<UMaterials>, public Tpl::TAsSingle<FMaterialsManager>
{
public:
	void Init();
	void Clear();

private:
	std::wstring mFileName = L"MaterialsManager.data";
};
