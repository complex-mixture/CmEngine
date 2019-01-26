#pragma once
#include "Util.h"
#include <map>
#include "ResourceManagerBase.h"

class UMesh;
class FMeshManager : public FResourceManagerBase<UMesh>, FNoncopyable
{
	FMeshManager() {}
public:
	static FMeshManager & Get()
	{
		static FMeshManager single;
		return single;
	}

	void Init();
	void Clear();

private:
	std::wstring mFileName = L"MeshManager.data";
};
