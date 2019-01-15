#include "MeshManager.h"
#include "Vertex.h"

int main()
{
	FMeshManager::Get().Init();

	FMeshManager::Get().AddStaticMesh("dragon.FBX", false, L"Dragon", L"Dragon.mesh");
	FMeshManager::Get().AddStaticMesh("SM_Arrows.FBX", true, L"Arrows", L"Arrows.mesh");
	FMeshManager::Get().AddStaticMesh("SM_Chair.FBX", true, L"Chair", L"Chair.mesh");
	FMeshManager::Get().AddStaticMesh("SM_CornerFrame.FBX", true, L"CornerFrame", L"CornerFrame.mesh");
	FMeshManager::Get().AddStaticMesh("SM_TableRound.FBX", true, L"TableRound", L"TableRound.mesh");
	FMeshManager::Get().AddStaticMesh("teapot.FBX", false, L"Teapot", L"Teapot.mesh");
	FMeshManager::Get().AddStaticMesh("Sphere.FBX", true, L"Sphere", L"Sphere.mesh");
	FMeshManager::Get().AddStaticMesh("SM_AssetPlatform.FBX", true, L"AssetPlatform", L"AssetPlatform.mesh");
	FMeshManager::Get().AddStaticMesh("Floor_400x400.FBX", true, L"Floor400x400", L"Floor400x400.mesh");
	FMeshManager::Get().AddStaticMesh("TranslateArrowHandle.FBX", true, L"Arrow", L"Arrow.mesh");

	FMeshManager::Get().AddStaticMesh<Vertex_SkyBox>("SkyBox.FBX", false, L"SkyBox", L"SkyBox.mesh");


	FMeshManager::Get().Save();
}
