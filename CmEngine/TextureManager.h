#pragma once
#include "ResourceManagerBase.h"
#include <d3d12.h>
#include "Util.h"
#include <string>
#include "Texture.h"
#include "Template.h"

class FTextureManager final : public FResourceManagerBase<UTexture>, public Tpl::TAsSingle<FTextureManager>
{
public:
	void Init();
	void Clear();

private:
	std::wstring mFileName = L"TextureManager.data";
};
