#include "MeshManager.h"

int main()
{
	FMeshManager::Get().Init();

	FMeshManager::Get().AddStaticMesh("dragon.FBX", L"dragon", L"dragon.mesh");
	FMeshManager::Get().AddStaticMesh("SM_Arrows.FBX", L"arrows", L"arrows.mesh");
	FMeshManager::Get().AddStaticMesh("SM_Chair.FBX", L"chair", L"chair.mesh");
	FMeshManager::Get().AddStaticMesh("SM_CornerFrame.FBX", L"cornerFrame", L"cornerFrame.mesh");

	FMeshManager::Get().Save();
}
