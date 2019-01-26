#include "World.h"
#include "CameraActor.h"
#include "StaticMeshActor.h"
#include "MeshManager.h"
#include "MaterialsManager.h"
#include "DirectionLightActor.h"
#include "PointLightActor.h"
#include "SpotLightActor.h"
#include <math.h>
#include "Common.h"
#include "TextureManager.h"
#include <windows.h>
#include "SkyBoxActor.h"
#pragma comment(lib, "ws2_32.lib")

void UWorld::BeginPlay()
{
	mCamera = new ACameraActor;
	mCamera->SetPosition(XMFLOAT3(0.f, 0.f, 120.f));
	mCamera->SetRotation(XMFLOAT3(0.f, -PI / 18.f, 0.f));
	//mCamera = new ACameraActor;
	//mCamera->SetPosition(XMFLOAT3(0.f, 0.f, 100.f));
	//mCamera->SetRotation(XMFLOAT3(0.f, PI / 2.f, 0.f));
	//mCamera->SetFarClipDistance(100.f);

	mSkyBox = new ASkyBoxActor;
	mSkyBox->SetStaticMesh(FMeshManager::Get().LoadResource(L"SkyBox"));
	mSkyBox->SetMaterials(FMaterialsManager::Get().LoadResource(L"SkyBox"));
	mSkyBox->SetMaterialsParameterDescriptorTable(0, FTextureManager::Get().LoadResource(L"SkyBoxB")->GetDescriptorHandle().mGpuHandle);

	AStaticMeshActor * newstaticMeshActor = new AStaticMeshActor;
	newstaticMeshActor->SetPosition(XMFLOAT3(130.f, 400.f, 0.f));
	newstaticMeshActor->SetScale(XMFLOAT3(2.f, 2.f, 2.f));
	newstaticMeshActor->SetStaticMesh(FMeshManager::Get().LoadResource(L"Floor400x400"));
	newstaticMeshActor->SetMaterials(FMaterialsManager::Get().LoadResource(L"Pi"));
	newstaticMeshActor->SetMaterialsParameterDescriptorTable(0, FTextureManager::Get().LoadResource(L"CobbleStoneRoughB")->GetDescriptorHandle().mGpuHandle);
	newstaticMeshActor->SetMaterialsParameterDescriptorTable(1, FTextureManager::Get().LoadResource(L"CobbleStoneRoughN")->GetDescriptorHandle().mGpuHandle);
	mActors.push_back(newstaticMeshActor);

	newstaticMeshActor = new AStaticMeshActor;
	newstaticMeshActor->SetPosition(XMFLOAT3(280, 0.f, 0.f));
	newstaticMeshActor->SetScale(XMFLOAT3(20.f, 20.f, 20.f));
	newstaticMeshActor->SetRotation(XMFLOAT3(0.f, 0.f, PI / 2.f));
	newstaticMeshActor->SetStaticMesh(FMeshManager::Get().LoadResource(L"Dragon"));
	newstaticMeshActor->SetMaterials(FMaterialsManager::Get().LoadResource(L"Pi"));
	newstaticMeshActor->SetMaterialsParameterDescriptorTable(0, FTextureManager::Get().LoadResource(L"DefaultN")->GetDescriptorHandle().mGpuHandle);
	newstaticMeshActor->SetMaterialsParameterDescriptorTable(1, FTextureManager::Get().LoadResource(L"DefaultN")->GetDescriptorHandle().mGpuHandle);
	mActors.push_back(newstaticMeshActor);

	for (int i = -200; i != 200; ++i)
	{
			newstaticMeshActor = new AStaticMeshActor;
			newstaticMeshActor->SetPosition(XMFLOAT3(/*280 + */i * 20.f, 0.f, 80.f));
			newstaticMeshActor->SetScale(XMFLOAT3(0.2f, 0.2f, 0.2f));
			newstaticMeshActor->SetRotation(XMFLOAT3( 0.f, 0.f,-PI));
			newstaticMeshActor->SetStaticMesh(FMeshManager::Get().LoadResource(L"Chair"));
			newstaticMeshActor->SetMaterials(FMaterialsManager::Get().LoadResource(L"Pi"));
			newstaticMeshActor->SetMaterialsParameterDescriptorTable(0, FTextureManager::Get().LoadResource(L"CobbleStoneRoughB")->GetDescriptorHandle().mGpuHandle);
			newstaticMeshActor->SetMaterialsParameterDescriptorTable(1, FTextureManager::Get().LoadResource(L"CobbleStoneRoughN")->GetDescriptorHandle().mGpuHandle);
			mActors.push_back(newstaticMeshActor);
	}

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
	mActors.push_back(newDirectionLight);

	APointLightActor * newPointLight = new APointLightActor;
	newPointLight->SetPosition(XMFLOAT3(240.f, -110.f, 50.f));
	newPointLight->SetColor(DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	newPointLight->SetIntensity(1000.f);
	newPointLight->SetFallOffBegin(1.f);
	newPointLight->SetFallOffEnd(200.f);
	mActors.push_back(newPointLight);

	ASpotLightActor * newSpotLight = new ASpotLightActor;
	newSpotLight->SetPosition(XMFLOAT3(220.f, 190.f, 10.f));
	newSpotLight->SetRotation(XMFLOAT3(0.f, 0.f, -PI / 3.f));
	newSpotLight->SetColor(DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	newSpotLight->SetIntensity(5000.f);
	newSpotLight->SetFallOffBegin(1.f);
	newSpotLight->SetFallOffEnd(1000.f);
	newSpotLight->SetInnerConeAngle(0.f);
	newSpotLight->SetOuterConeAngle(PI / 3.f);
	mActors.push_back(newSpotLight);

	mRecvThread = std::thread([&]() {
		WSADATA D = {};
		WSAStartup(MAKEWORD(2, 2), &D);

		SOCKET sock_eng = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		char recvBuffer[1024 * 6];

		sockaddr addr;
		addr.sa_family = AF_INET;
		memset(addr.sa_data, 0x00, 14);
		//addr.sin_family = AF_INET;
		//addr.sin_addr.S_un.S_addr = 0ul;
		//addr.sin_port = htons(2333);

		while (true)
		{
			int formLen = sizeof(addr);
			int recvcount = recvfrom(sock_eng, recvBuffer, 1024 * 6, 0, &addr, &formLen);
			if (recvcount < 24 || recvcount == SOCKET_ERROR) continue;
			memcpy(mLastPosition, recvBuffer + (recvcount / 24 - 1) * 24, 12);
			memcpy(mLastRotation, recvBuffer + (recvcount / 24 - 1) * 24 + 12, 12);
		}
		closesocket(sock_eng);
	});
}

void UWorld::Tick(float _deltaTime)
{
	//for (auto _ele : mStaticMeshActors)
	//	_ele->Tick(_deltaTime);
	//for (auto _ele : mDirectionLights)
	//	_ele->Tick(_deltaTime);
	//for (auto _ele : mPointLights)
	//	_ele->Tick(_deltaTime);
	//for (auto _ele : mSpotLights)
	//	_ele->Tick(_deltaTime);

	//XMFLOAT3 b = mCamera->GetRotation();
	//b.y -= _deltaTime/* * 10.f*/;
	//mCamera->SetRotation(b);

	//for (int i = 0; i != mSpotLights.size(); ++i)
	//{
	//	XMFLOAT3 b = mSpotLights[i]->GetRotation();
	//	b.z -= _deltaTime;
	//	mSpotLights[i]->SetRotation(b);
	//}
}

void UWorld::EndPlay()
{
	for (auto _ele : mActors)
		_ele->EndPlay();
	mCamera->EndPlay();

	delete mCamera;
	for (auto _ele : mActors)
		delete _ele;

	mActors.resize(0);
}
