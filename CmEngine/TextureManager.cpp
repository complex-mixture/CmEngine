#include "TextureManager.h"
#include "RenderModule.h"

void FTextureManager::Init()
{
	std::ifstream fs(mFileName, std::ios::in | std::ios::binary);

	uint64_t elementCount = 0;
	fs.read(reinterpret_cast<char*>(&elementCount), sizeof(elementCount));
	std::wstring meshName;
	std::wstring fileName;
	for (uint64_t i = 0; i != elementCount; ++i)
	{
		LoadStringFromFile<wchar_t>(fs, meshName);
		LoadStringFromFile<wchar_t>(fs, fileName);
		UTexture * newTexture = new UTexture();
		newTexture->Init(fileName);
		mMap.insert(std::make_pair(meshName, newTexture));
	}
	fs.close();

	GetRenderModule()->AddRenderThreadTask([&]() {
		for (auto _ele : mMap)
			_ele.second->Commit();
	}
	);

	GetRenderModule()->AddTaskOnRenderThreadFlush([&]() {
		for (auto _ele : mMap)
			_ele.second->EndCommit();
	}
	);
}

void FTextureManager::Clear()
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
