#include <windows.h>
#include "EngineLoop.h"
#include <exception>
#include "Util.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	try
	{
		GEngineLoop.PreInit(hInstance, lpCmdLine);
		GEngineLoop.Init();
		while (!GEngineLoop.ShouldExit())
		{
			GEngineLoop.Tick();
		}
		GEngineLoop.Exit();
	}
	catch (std::exception& e)
	{
		DebugMessageBoxA("exception", e.what());
	}
}
