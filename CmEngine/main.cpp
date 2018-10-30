#include <windows.h>
#include "EngineLoop.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	GEngineLoop.PreInit(hInstance, lpCmdLine);
	GEngineLoop.Init();
	while (!GEngineLoop.RequestExit())
	{
		GEngineLoop.Tick();
	}
	GEngineLoop.Exit();
}
