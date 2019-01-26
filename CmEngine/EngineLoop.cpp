#include "EngineLoop.h"
#include "ModuleManager.h"
#include "Engine.h"
#include <stdlib.h>
#include "WindowManager.h"
#include <thread>
#include "Global.h"
#include "RenderModule.h"
#include "D3D12RhiModule.h"

FEngineLoop GEngineLoop;
uint64_t GFrameCount = 0;

void FEngineLoop::PreInit(HINSTANCE hInstance, LPWSTR lpCmdLine)
{
	{
		mInstance = hInstance;
		mCmdLine = lpCmdLine;
	}
	GGameThreadId = std::this_thread::get_id();
	FModuleManager::Get().LoadModule(L"D3D12Rhi");
	FModuleManager::Get().LoadModule(L"SlateCore");
	FModuleManager::Get().LoadModule(L"Render");
	FWindowManager::Get().Init(mInstance);
}

void FEngineLoop::Init()
{
	GEngine = new FEngine;
	GetRenderModule()->BeginFrame_GameThread();
	GEngine->Init();
	GetRenderModule()->CollectRenderFrameResource();
	GetRenderModule()->EndFrame_GameThead();
	mTimer.Reset();
}

void FEngineLoop::Tick()
{
	mTimer.Tick();
	GetRenderModule()->BeginFrame_GameThread();
	FWindowManager::Get().PumpMessages();
	GEngine->Tick(mTimer.DeltaTime());
	GetRenderModule()->CollectRenderFrameResource();
	GetRenderModule()->EndFrame_GameThead();
	GFrameCount++;
}

void FEngineLoop::Exit()
{
	GetRenderModule()->BeginFrame_GameThread();
	GEngine->Exit();
	GetRenderModule()->CollectRenderFrameResource();
	GetRenderModule()->EndFrame_GameThead();
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
