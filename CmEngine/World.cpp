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
	mCamera->SetPosition(XMFLOAT3(-100.f, 0.f, 0.f));
	mCamera->SetRotation(XMFLOAT3(0.f, 0.f, 0.f));
	AStaticMeshActor * newstaticMeshActor = new AStaticMeshActor;
s	newstaticMeshActor->SetStaticMesh(FStaticMeshManager::Get().LoadResource(L"SkyBox"));
	newstaticMeshActor->SetMaterials(FMaterialsManager::Get().LoadResource(L"SkyBox"));
	newstaticMeshActor->SetMaterialsParameterDescriptorTable(0, FTextureManager::Get().LoadResource(L"SkyBoxB")->GetDescriptorHandle().mGpuHandle);
	mStaticMeshActors.push_back(newstaticMeshActor);

	AStaticMeshActor * newstaticMeshActor2 = new AStaticMeshActor;
	newstaticMeshActor2->SetRotation(XMFLOAT3(0.f, 0.f, 1.f));
	newstaticMeshActor2->SetStaticMesh(FStaticMeshManager::Get().LoadResource(L"Chair"));
	newstaticMeshActor2->SetMaterials(FMaterialsManager::Get().LoadResource(L"GoochChair"));
	newstaticMeshActor2->SetMaterialsParameterDescriptorTable(0, FTextureManager::Get().LoadResource(L"ChairB")->GetDescriptorHandle().mGpuHandle);
	newstaticMeshActor2->SetMaterialsParameterDescriptorTable(1, FTextureManager::Get().LoadResource(L"ChairN")->GetDescriptorHandle().mGpuHandle);
	mStaticMeshActors.push_back(newstaticMeshActor2);

	ADirectionLightActor * newDirectionLight = new ADirectionLightActor;
	newDirectionLight->SetRotation(DirectX::XMFLOAT3(-0.f, 0.f, PI / 2));
	newDirectionLight->SetColor(DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	newDirectionLight->SetIntensity(1.f);
	mDirectionLights.push_back(newDirectionLight);

	ASpotLightActor * newSpotLight = new ASpotLightActor;
	newSpotLight->SetRotation(DirectX::XMFLOAT3(0.f, PI / 4.f, -PI / 4.f));
	newSpotLight->SetColor(DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	newSpotLight->SetIntensity(1.f);
	newSpotLight->SetInnerConeAngle(PI / 6.F);
	newSpotLight->SetOuterConeAngle(PI / 3.F);

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

	XMFLOAT3 b = mCamera->GetRotation();
	b.z = fmod(b.z + _deltaTime * 1.f, PI2);
	mCamera->SetRotation(b);


	for (int i = 0; i != mStaticMeshActors.size(); ++i)
	{
		XMFLOAT3 b = mStaticMeshActors[i]->GetRotation();
		b.z = fmod(b.z + _deltaTime * 1.f, PI2);
		mStaticMeshActors[i]->SetRotation(b);
	}
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
