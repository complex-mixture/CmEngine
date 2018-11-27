#include "Engine.h"
#include "Window.h"
#include "RenderModule.h"

FEngine * GEngine = nullptr;

void FEngine::Init()
{
	mClientWindow = new FWindow(0, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, L"Äã»áàÓàÓàÓÃ´111");
	mClientWindow2 = new FWindow(0, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, L"Äã»áàÓàÓàÓÃ´222");
}

void FEngine::Tick(float _deltaTime)
{
	if (!mClientWindow->IsClosed())
		GetRenderModule()->DeferCollectRenderFrameResource(mClientWindow->GetViewport(), FRenderSetting(), nullptr);
	if (!mClientWindow2->IsClosed())
		GetRenderModule()->DeferCollectRenderFrameResource(mClientWindow2->GetViewport(), FRenderSetting(), nullptr);
}

void FEngine::Exit()
{
}
