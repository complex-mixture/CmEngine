#include "EngineLoop.h"
#include "ModuleManager.h"
#include "Engine.h"
#include <stdlib.h>
#include "WindowManager.h"
#include <thread>
#include "Global.h"
#include "RenderModule.h"

FEngineLoop GEngineLoop;
uint64_t GFrameCount = 0;

void FEngineLoop::PreInit(HINSTANCE hInstance, LPWSTR lpCmdLine)
{
	{
		mInstance = hInstance;
		mCmdLine = lpCmdLine;
	}
	GGameThreadId == std::this_thread::get_id();
	FModuleManager::Get().LoadModule(L"D3D12Rhi");
	FModuleManager::Get().LoadModule(L"SlateCore");
	FModuleManager::Get().LoadModule(L"Render");
	FWindowManager::Get().Init(mInstance);
}

void FEngineLoop::Init()
{
	GEngine = new FEngine;
	GEngine->Init();
	mTimer.Start();
}

void FEngineLoop::Tick()
{
	mTimer.Tick();
	FWindowManager::Get().PumpMessages();
	GetRenderModule()->BeginFrame_GameThread();
	GEngine->Tick(mTimer.DeltaTime());
	GetRenderModule()->CollectRenderFrameResource();
	GetRenderModule()->EndFrame_GameThead();
	GFrameCount++;
}

void FEngineLoop::Exit()
{
	GEngine->Exit();
	delete GEngine;
	FModuleManager::Get().UnloadModule(L"Render");
	FModuleManager::Get().UnloadModule(L"SlateCore");
	FModuleManager::Get().UnloadModule(L"D3D12Rhi");
	FModuleManager::Get().Exit();
}

void FEngineLoop::RequestExit(bool _force)
{
	if (_force) abort();
	bShouldExit = true;
	PostQuitMessage(0);
}

void RequestExit(bool _force)
{
	GEngineLoop.RequestExit(_force);
}
