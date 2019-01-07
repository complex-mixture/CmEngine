#include "MaterialsManager.h"
#include "Materials.h"
#include "RenderModule.h"

void FMaterialsManager::Init()
{
	std::ifstream fs(mFileName, std::ios::in | std::ios::binary);

	uint64_t elementCount = 0;
	fs.read(reinterpret_cast<char*>(&elementCount), sizeof(elementCount));
	std::wstring meshName;
	std::wstring fileName;
	for (uint64_t i = 0; i != elementCount; ++i)
	{
		LoadStringFromFile(fs, meshName);
		LoadStringFromFile(fs, fileName);
		UMaterials * newMaterials = new UMaterials;
		newMaterials->Init(fileName);
		mMap.insert(std::make_pair(meshName, newMaterials));
	}
	fs.close();
}

void FMaterialsManager::Clear()
{
	GetRenderModule()->AddTaskOnRenderThreadFlush([&]() {
		for (auto & _ele : mMap)
		{
			_ele.second->Clear();
			delete _ele.second;
			_ele.second = nullptr;
		}
		mMap.clear();
	});
}
