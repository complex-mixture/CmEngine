#pragma once
#include "ResourceManagerBase.h"
#include <d3d12.h>
#include "Util.h"
#include <string>
#include "Texture.h"

class FTextureManager : public FResourceManagerBase<UTexture>, FNoncopyable
{
public:
	static FTextureManager & Get()
	{
		static FTextureManager single;
		return single;
	}

	void Init();
	void Clear();

private:
	std::wstring mFileName = L"TextureManager.data";
};