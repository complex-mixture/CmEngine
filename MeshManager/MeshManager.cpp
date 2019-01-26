#include "MeshManager.h"
#include <fstream>
#include <filesystem>
#include <stdint.h>
#include "Util.h"
#include <fbxsdk.h>
#include "Mesh.h"
#pragma comment(lib, "libfbxsdk.lib")

void FMeshManager::Init()
{
	std::ifstream ifs(mManagerFileName, std::ios::in | std::ios::binary);
	if (ifs.is_open())
	{
		uint64_t elementCount = 0;
		ifs.read(reinterpret_cast<char*>(&elementCount), sizeof(uint64_t));
		std::wstring meshName;
		std::wstring fileName;
		for (uint64_t i = 0; i != elementCount; ++i)
		{
			LoadStringFromFile<wchar_t>(ifs, meshName);
			LoadStringFromFile<wchar_t>(ifs, fileName);
			mMap.insert(std::make_pair(meshName, fileName));
		}
	}
	else
	{
		LogW(L"File (%s) not exit", mManagerFileName.c_str());
	}

	mFbxManager = FbxManager::Create();
	mDefaultIoSetting = FbxIOSettings::Create(mFbxManager, nullptr);
	mFbxImporter = FbxImporter::Create(mFbxManager, nullptr);
}

void FMeshManager::Save()
{
	std::ofstream ofs(mManagerFileName, std::ios::out | std::ios::binary);
	uint64_t elementCount = mMap.size();
	ofs.write(reinterpret_cast<const char*>(&elementCount), sizeof(uint64_t));
	for (auto const & _ele : mMap)
	{
		SaveStringToFile(ofs, _ele.first);
		SaveStringToFile(ofs, _ele.second);
	}
}

void FMeshManager::Clear()
{
	Save();
	mFbxImporter->Destroy();
	mFbxManager->Destroy();
}
