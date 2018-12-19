#pragma once
#include "Util.h"
#include <map>
#include "ResourceManagerBase.h"
class UMaterials;

class FMaterialsManager : public FResourceManagerBase<UMaterials>, FNoncopyable
{
	FMaterialsManager() {}
public:
	static FMaterialsManager & Get()
	{
		static FMaterialsManager single;
		return single;
	}

	void Init();
	void Clear();

private:
	std::wstring mFileName = L"MaterialsManager.data";
};
