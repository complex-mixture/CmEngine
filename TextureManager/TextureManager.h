#pragma once
#include <string>
#include <map>
#include <vector>
#include <utility>

class FTextureManager
{
public:
	static FTextureManager & Get()
	{
		static FTextureManager single;
		return single;
	}

	void Init();
	void Save();
	void Clear();

	void AddTexture(std::wstring _ddsFileName, std::wstring _textureName, std::wstring _destFileName);
	void EraseTexture(std::wstring _textureName)
	{
		mMap.erase(_textureName);
	}

private:
	std::map<std::wstring, std::wstring> mMap;
	std::wstring mManagerFileName = L"TextureManager.data";
};