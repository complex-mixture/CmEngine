#include "TextureManager.h"

int main()
{
	//FTextureManager::Get().Init();

	FTextureManager::Get().AddTexture(L"CobbleStone_RoughB.dds", L"CobbleStoneRoughB", L"CobbleStoneRoughB.texture");
	FTextureManager::Get().AddTexture(L"SkyBoxB.dds", L"SkyBoxB", L"SkyBoxB.texture");
	FTextureManager::Get().AddTexture(L"ChairB.dds", L"ChairB", L"ChairB.texture");
	FTextureManager::Get().AddTexture(L"CobbleStone_RoughN.dds", L"CobbleStoneRoughN", L"CobbleStoneRoughN.texture");
	FTextureManager::Get().AddTexture(L"DefaultN.dds", L"DefaultN", L"DefaultN.texture");
	FTextureManager::Get().AddTexture(L"TableRoundB.dds", L"TableRoundB", L"TableRoundB.texture");
	FTextureManager::Get().AddTexture(L"ChairN.dds", L"ChairN", L"ChairN.texture");

	FTextureManager::Get().Clear();
}