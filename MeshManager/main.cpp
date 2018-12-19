#include "MeshManager.h"

int main()
{
	FMeshManager::Get().Init();
	FMeshManager::Get().AddStaticMesh<Vertex_default>("Box.FBX", L"box", L"box.mesh");
	FMeshManager::Get().AddStaticMesh<Vertex_default>("Teapot.FBX", L"teapot", L"teapot.mesh");
	FMeshManager::Get().AddStaticMesh<Vertex_default>("dragon.FBX", L"dragon", L"dragon.mesh");
	FMeshManager::Get().AddStaticMesh<Vertex_default>("Sphere.FBX", L"sphere", L"sphere.mesh");
	FMeshManager::Get().Save();
}
