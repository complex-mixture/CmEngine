#include "MeshManager.h"
#include "Vertex.h"

int main()
{
	FMeshManager::Get().Init();

	FMeshManager::Get().AddMesh("dragon.FBX", false, L"Dragon", L"Dragon.mesh");
	FMeshManager::Get().AddMesh("SM_Arrows.FBX", true, L"Arrows", L"Arrows.mesh");
	FMeshManager::Get().AddMesh("SM_Chair.FBX", true, L"Chair", L"Chair.mesh");
	FMeshManager::Get().AddMesh("SM_CornerFrame.FBX", true, L"CornerFrame", L"CornerFrame.mesh");
	FMeshManager::Get().AddMesh("SM_TableRound.FBX", true, L"TableRound", L"TableRound.mesh");
	FMeshManager::Get().AddMesh("teapot.FBX", false, L"Teapot", L"Teapot.mesh");
	FMeshManager::Get().AddMesh("Sphere.FBX", true, L"Sphere", L"Sphere.mesh");
	FMeshManager::Get().AddMesh("SM_AssetPlatform.FBX", true, L"AssetPlatform", L"AssetPlatform.mesh");
	FMeshManager::Get().AddMesh("Floor_400x400.FBX", true, L"Floor400x400", L"Floor400x400.mesh");
	FMeshManager::Get().AddMesh("TranslateArrowHandle.FBX", true, L"Arrow", L"Arrow.mesh");

	FMeshManager::Get().AddMesh<Vertex_SkyBox>("SkyBox.FBX", false, L"SkyBox", L"SkyBox.mesh");


	FMeshManager::Get().Save();
}
