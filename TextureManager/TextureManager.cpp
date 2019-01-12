#include "TextureManager.h"
#include "Util.h"
#include "Texture.h"

void FTextureManager::Init()
{
	std::ifstream ifs(mManagerFileName, std::ios::in | std::ios::binary);
	if (ifs.is_open())
	{
		uint64_t elementCount = 0;
		ifs.read(reinterpret_cast<char*>(&elementCount), sizeof(uint64_t));
		std::wstring textureName;
		std::wstring fileName;
		for (uint64_t i = 0; i != elementCount; ++i)
		{
			LoadStringFromFile<wchar_t>(ifs, textureName);
			LoadStringFromFile<wchar_t>(ifs, fileName);
			mMap.insert(std::make_pair(textureName, fileName));
		}
	}
	else
	{
		LogW(L"File (%s) not exit", mManagerFileName.c_str());
	}
}

void FTextureManager::Save()
{
	std::ofstream ofs(mManagerFileName, std::ios::out | std::ios::binary);
	uint64_t elementCount = mMap.size();
	ofs.write(reinterpret_cast<const char*>(&elementCount), sizeof(uint64_t));
	for (auto const & _ele : mMap)
	{
		SaveStringToFile<wchar_t>(ofs, _ele.first);
		SaveStringToFile<wchar_t>(ofs, _ele.second);
	}
}

void FTextureManager::Clear()
{
	Save();
}

void FTextureManager::AddTexture(std::wstring _ddsFileName, std::wstring _textureName, std::wstring _destFileName)
{
	FTexture texture;
	texture.Construct(_ddsFileName);
	texture.SaveToFile(_destFileName);
	mMap.insert(std::make_pair(_textureName, _destFileName));
}
