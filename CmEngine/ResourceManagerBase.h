#pragma once
#include <string>
#include <map>
template<typename _resourceType>
class FResourceManagerBase
{
public:
	using ResourceType = _resourceType;

	void Init() = delete;
	void Clear() = delete;

	ResourceType * LoadMesh(std::wstring _name)
	{
		auto it = mMap.find(_name);
		if (it == mMap.end())
			return nullptr;
		return it->second;
	}

protected:
	FResourceManagerBase() {}
	std::map<std::wstring, ResourceType*> mMap;
};