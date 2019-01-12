#include "TextureManager.h"

int main()
{
	//FTextureManager::Get().Init();

	FTextureManager::Get().AddTexture(L"BricksB.dds", L"BricksB", L"BricksB.texture");
	FTextureManager::Get().AddTexture(L"SkyBoxB.dds", L"SkyBoxB", L"SkyBoxB.texture");
	FTextureManager::Get().AddTexture(L"ChairB.dds", L"ChairB", L"ChairB.texture");
	FTextureManager::Get().AddTexture(L"BricksN.dds", L"BricksN", L"BricksN.texture");
	FTextureManager::Get().AddTexture(L"DefaultN.dds", L"DefaultN", L"DefaultN.texture");
	FTextureManager::Get().AddTexture(L"TableRoundB.dds", L"TableRoundB", L"TableRoundB.texture");
	FTextureManager::Get().AddTexture(L"ChairN.dds", L"ChairN", L"ChairN.texture");

	FTextureManager::Get().Clear();
}