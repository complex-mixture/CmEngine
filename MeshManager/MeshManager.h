#pragma once
#include <map>
#include <string>
#include <fbxsdk.h>
#include "Vertex.h"
#include "Util.h"
#include "Mesh.h"
#include "MeshLoader.h"

class FMeshManager
{
	FMeshManager() = default;
public:
	static FMeshManager & Get()
	{
		static FMeshManager single;
		return single;
	}

	void Init();
	void Save();
	void Clear();

	template<typename _vertexType = Vertex_default>
	void AddMesh(std::string _fbxFileName, bool _isExportFromUnreal, const std::wstring & _meshName, const std::wstring & _destFileName);

	void EraseMesh(const std::wstring & _meshName)
	{
		mMap.erase(_meshName);
	}

private:
	FbxManager * mFbxManager = nullptr;
	FbxIOSettings * mDefaultIoSetting = nullptr;
	FbxImporter * mFbxImporter = nullptr;
	std::map<std::wstring, std::wstring> mMap;
	std::wstring mManagerFileName = L"MeshManager.data";
};

template<typename _vertexType>
inline void FMeshManager::AddMesh(std::string _fbxFileName, bool _isExportFromUnreal, const std::wstring & _meshName, const std::wstring & _destFileName)
{
	if (!mFbxImporter->Initialize(_fbxFileName.c_str()))
	{
		DebugMessageBoxW(L"AddMesh failed", L"Error returned :%S", mFbxImporter->GetStatus().GetErrorString());
	}

	FbxScene * fbxScene = FbxScene::Create(mFbxManager, nullptr);
	mFbxImporter->Import(fbxScene);

	FbxNode * rootNode = fbxScene->GetRootNode();
	Assert(rootNode && rootNode->GetChildCount() == 1 && rootNode->GetChild(0)->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh);
	
	LogW(L"[%s : %s]\n", _meshName.c_str(), _destFileName.c_str());
	
	UMesh<_vertexType> Mesh;
	Mesh.Construct(rootNode->GetChild(0)->GetMesh(), _isExportFromUnreal);
	Mesh.SaveToFile(_destFileName);
	fbxScene->Destroy();
	mMap.insert(std::make_pair(_meshName, _destFileName));
}
