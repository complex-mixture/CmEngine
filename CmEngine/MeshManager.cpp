#include "MeshManager.h"
#include <fstream>
#include "Mesh.h"
#include "RenderModule.h"

void FMeshManager::Init()
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
		UMesh * newMesh = new UMesh();
		newMesh->Init(fileName);
		mMap.insert(std::make_pair(meshName, newMesh));
	}
	fs.close();

	GetRenderModule()->AddRenderThreadTask([&]() {
		for (auto _ele : mMap)
			_ele.second->CommitResource();
	}
	);

	GetRenderModule()->AddTaskOnRenderThreadFlush([&]() {
		for (auto _ele : mMap)
			_ele.second->EndCommit();
	}
	);
}

void FMeshManager::Clear()
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
