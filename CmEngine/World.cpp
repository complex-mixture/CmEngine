#include "World.h"
#include "CameraActor.h"
#include "StaticMeshActor.h"
#include "StaticMeshManager.h"
#include "MaterialsManager.h"
#include "DirectionLightActor.h"
#include "PointLightActor.h"
#include "SpotLightActor.h"
#include <math.h>
#include "Common.h"
#include "TextureManager.h"

void UWorld::BeginPlay()
{
	mCamera = new ACameraActor;
	mCamera->SetPosition(XMFLOAT3(0.f, 0.f, 120.f));
	mCamera->SetRotation(XMFLOAT3(0.f, -PI / 18.f, 0.f));

	AStaticMeshActor * newstaticMeshActor = new AStaticMeshActor;
	newstaticMeshActor->SetStaticMesh(FStaticMeshManager::Get().LoadResource(L"SkyBox"));
	newstaticMeshActor->SetMaterials(FMaterialsManager::Get().LoadResource(L"SkyBox"));
	newstaticMeshActor->SetMaterialsParameterDescriptorTable(0, FTextureManager::Get().LoadResource(L"SkyBoxB")->GetDescriptorHandle().mGpuHandle);
	mStaticMeshActors.push_back(newstaticMeshActor);

	newstaticMeshActor = new AStaticMeshActor;
	newstaticMeshActor->SetPosition(XMFLOAT3(130.f, 400.f, 0.f));
	newstaticMeshActor->SetScale(XMFLOAT3(2.f, 2.f, 2.f));
	newstaticMeshActor->SetStaticMesh(FStaticMeshManager::Get().LoadResource(L"Floor400x400"));
	newstaticMeshActor->SetMaterials(FMaterialsManager::Get().LoadResource(L"Pi"));
	newstaticMeshActor->SetMaterialsParameterDescriptorTable(0, FTextureManager::Get().LoadResource(L"CobbleStoneRoughB")->GetDescriptorHandle().mGpuHandle);
	newstaticMeshActor->SetMaterialsParameterDescriptorTable(1, FTextureManager::Get().LoadResource(L"CobbleStoneRoughN")->GetDescriptorHandle().mGpuHandle);
	mStaticMeshActors.push_back(newstaticMeshActor);

	newstaticMeshActor = new AStaticMeshActor;
	newstaticMeshActor->SetPosition(XMFLOAT3(280, 0.f, 0.f));
	newstaticMeshActor->SetScale(XMFLOAT3(20.f, 20.f, 20.f));
	newstaticMeshActor->SetRotation(XMFLOAT3(0.f, 0.f, PI / 2.f));
	newstaticMeshActor->SetStaticMesh(FStaticMeshManager::Get().LoadResource(L"Dragon"));
	newstaticMeshActor->SetMaterials(FMaterialsManager::Get().LoadResource(L"Pi"));
	newstaticMeshActor->SetMaterialsParameterDescriptorTable(0, FTextureManager::Get().LoadResource(L"DefaultN")->GetDescriptorHandle().mGpuHandle);
	newstaticMeshActor->SetMaterialsParameterDescriptorTable(1, FTextureManager::Get().LoadResource(L"DefaultN")->GetDescriptorHandle().mGpuHandle);
	mStaticMeshActors.push_back(newstaticMeshActor);

	//newstaticMeshActor = new AStaticMeshActor;
	//newstaticMeshActor->SetPosition(XMFLOAT3(240.f, -110.f, 50.f));
	//newstaticMeshActor->SetScale(XMFLOAT3(0.2f, 0.2f, 0.2f));
	//newstaticMeshActor->SetStaticMesh(FStaticMeshManager::Get().LoadResource(L"Sphere"));
	//newstaticMeshActor->SetMaterials(FMaterialsManager::Get().LoadResource(L"DebugUv"));
	//mStaticMeshActors.push_back(newstaticMeshActor);

	//newstaticMeshActor = new AStaticMeshActor;
	//newstaticMeshActor->SetPosition(XMFLOAT3(220.f, 190.f, 10.f));
	//newstaticMeshActor->SetScale(XMFLOAT3(2.f, 2.f, 2.f));
	//newstaticMeshActor->SetRotation(XMFLOAT3(0.f, 0.f, -PI / 3.f));
	//newstaticMeshActor->SetStaticMesh(FStaticMeshManager::Get().LoadResource(L"Arrow"));
	//newstaticMeshActor->SetMaterials(FMaterialsManager::Get().LoadResource(L"DebugUv"));
	//mStaticMeshActors.push_back(newstaticMeshActor);

	//newstaticMeshActor = new AStaticMeshActor;
	//newstaticMeshActor->SetPosition(XMFLOAT3(220.f, 190.f, 50.f));
	//newstaticMeshActor->SetScale(XMFLOAT3(0.05f, 0.05f, 0.05f));
	//newstaticMeshActor->SetRotation(DirectX::XMFLOAT3(0.f, PI / 2, 0.f));
	//newstaticMeshActor->SetStaticMesh(FStaticMeshManager::Get().LoadResource(L"Arrows"));
	//newstaticMeshActor->SetMaterials(FMaterialsManager::Get().LoadResource(L"DebugUv"));
	//mStaticMeshActors.push_back(newstaticMeshActor);

	ADirectionLightActor * newDirectionLight = new ADirectionLightActor;
	newDirectionLight->SetRotation(DirectX::XMFLOAT3(0.f, PI / 2, 0.f));
	newDirectionLight->SetColor(DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	newDirectionLight->SetIntensity(0.2f);
	mDirectionLights.push_back(newDirectionLight);

	APointLightActor * newPointLight = new APointLightActor;
	newPointLight->SetPosition(XMFLOAT3(240.f, -110.f, 50.f));
	newPointLight->SetColor(DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	newPointLight->SetIntensity(1000.f);
	newPointLight->SetFallOffBegin(1.f);
	newPointLight->SetFallOffEnd(200.f);
	mPointLights.push_back(newPointLight);

	ASpotLightActor * newSpotLight = new ASpotLightActor;
	newSpotLight->SetPosition(XMFLOAT3(220.f, 190.f, 10.f));
	newSpotLight->SetRotation(XMFLOAT3(0.f, 0.f, -PI / 3.f));
	newSpotLight->SetColor(DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	newSpotLight->SetIntensity(5000.f);
	newSpotLight->SetFallOffBegin(1.f);
	newSpotLight->SetFallOffEnd(1000.f);
	newSpotLight->SetInnerConeAngle(0.f);
	newSpotLight->SetOuterConeAngle(PI / 3.f);
	mSpotLights.push_back(newSpotLight);
}

void UWorld::Tick(float _deltaTime)
{
	for (auto _ele : mStaticMeshActors)
		_ele->Tick(_deltaTime);
	for (auto _ele : mDirectionLights)
		_ele->Tick(_deltaTime);
	for (auto _ele : mPointLights)
		_ele->Tick(_deltaTime);
	for (auto _ele : mSpotLights)
		_ele->Tick(_deltaTime);

	//XMFLOAT3 b = mCamera->GetRotation();
	//b.z = fmod(b.z + _deltaTime * 1.f, PI2);
	//mCamera->SetRotation(b);

	//for (int i = 0; i != mSpotLights.size(); ++i)
	//{
	//	XMFLOAT3 b = mSpotLights[i]->GetRotation();
	//	b.z = fmod(b.z + _deltaTime * 1.f, PI2);
	//	mSpotLights[i]->SetRotation(b);
	//}
}

void UWorld::EndPlay()
{
	delete mCamera;
	for (auto _ele : mStaticMeshActors)
		delete _ele;
	for (auto _ele : mDirectionLights)
		delete _ele;
	for (auto _ele : mPointLights)
		delete _ele;
	for (auto _ele : mSpotLights)
		delete _ele;

	mStaticMeshActors.resize(0);
	mDirectionLights.resize(0);
	mPointLights.resize(0);
	mSpotLights.resize(0);
}
