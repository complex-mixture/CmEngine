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

	FMeshManager::Get().AddStaticMesh<Vertex_SkyBox>("SkyBox.FBX", false, L"SkyBox", L"SkyBox.mesh");


	FMeshManager::Get().Save();
}
