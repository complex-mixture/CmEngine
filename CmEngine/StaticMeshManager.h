#pragma once
#include "Util.h"
#include <map>
#include "ResourceManagerBase.h"

class UStaticMesh;
class FStaticMeshManager : public FResourceManagerBase<UStaticMesh>, FNoncopyable
{
	FStaticMeshManager() {}
public:
	static FStaticMeshManager & Get()
	{
		static FStaticMeshManager single;
		return single;
	}

	void Init();
	void Clear();

private:
	std::wstring mFileName = L"StaticMeshManager.data";
};
