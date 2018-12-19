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

void UWorld::BeginPlay()
{
	mCamera = new ACameraActor;
	mCamera->SetPosition(XMFLOAT3(-50.f, 0.f, 0.f));
	mCamera->SetRotation(XMFLOAT3(0.f, 0.f, 0.f));
	AStaticMeshActor * newstaticMeshActor = new AStaticMeshActor;
	newstaticMeshActor->SetPosition(XMFLOAT3(0.f, 0.f, 50.f));
	newstaticMeshActor->SetStaticMesh(FStaticMeshManager::Get().LoadMesh(L"box"));
	newstaticMeshActor->SetMaterials(FMaterialsManager::Get().LoadMesh(L"defaultmaterials"));
	//mStaticMeshActors.push_back(newstaticMeshActor);

	AStaticMeshActor * newstaticMeshActor2 = new AStaticMeshActor;
	newstaticMeshActor2->SetScale(XMFLOAT3(3.f, 3.f, 3.f));
	newstaticMeshActor2->SetStaticMesh(FStaticMeshManager::Get().LoadMesh(L"dragon"));
	newstaticMeshActor2->SetMaterials(FMaterialsManager::Get().LoadMesh(L"defaultmaterials"));
	mStaticMeshActors.push_back(newstaticMeshActor2);

	ADirectionLightActor * newDirectionLight = new ADirectionLightActor;
	newDirectionLight->SetRotation(DirectX::XMFLOAT3(-0.5f, -0.9f, -1.f));
	newDirectionLight->SetColor(DirectX::XMFLOAT3(0.2f, 0.7f, 0.1f));
	newDirectionLight->SetIntensity(1.f);
	mDirectionLights.push_back(newDirectionLight);
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

	XMFLOAT3 a = mStaticMeshActors[0]->GetRotation();
	//a.x = fmod(a.x + _deltaTime * 0.5f, PI2);
	//a.y = fmod(a.y + _deltaTime * 0.5f, PI2);
	a.z = fmod(a.z + _deltaTime * 1.f, PI2);
	mStaticMeshActors[0]->SetRotation(a);
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