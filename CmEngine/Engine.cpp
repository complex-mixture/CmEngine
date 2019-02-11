#include "Engine.h"
#include "Window.h"
#include "RenderModule.h"
#include "Canvas.h"
#include "D3D12RhiModule.h"
#include "MeshManager.h"
#include "World.h"
#include "MaterialsManager.h"
#include "TextureManager.h"

FEngine * GEngine = nullptr;

void FEngine::Init()
{
	//mClientWindow = new FWindow(0, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 200, L"你会嘤嘤嘤么111");
	mClientWindow2 = new FWindow(0, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 2560, 1440, L"想要做你的小猫猫");
	FMaterialsManager::Get().Init();
	FMeshManager::Get().Init();
	FTextureManager::Get().Init();
	mMainWorld = new UWorld;
	mMainWorld->BeginPlay();
}

void FEngine::Tick(float _deltaTime)
{
	mMainWorld->Tick(_deltaTime);
	//if (!mClientWindow->IsClosed())
	//	GetRenderModule()->DeferCollectRenderFrameResource(FCanvas::Make(mClientWindow->GetViewport()), mMainWorld, FRenderSetting());
	if (!mClientWindow2->IsClosed())
		GetRenderModule()->DeferCollectRenderFrameResource(FCanvas::Make(mClientWindow2->GetViewport()), mMainWorld, FRenderSetting());
}

void FEngine::Exit()
{
	mMainWorld->EndPlay();
	FMeshManager::Get().Clear();
	FTextureManager::Get().Clear();
	FMaterialsManager::Get().Clear();
}
