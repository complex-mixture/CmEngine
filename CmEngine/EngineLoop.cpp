#include "EngineLoop.h"
#include "ModuleManager.h"

FEngineLoop GEngineLoop;

void FEngineLoop::PreInit(HINSTANCE hInstance, LPSTR lpCmdLine)
{
	{
		mInstance = hInstance;
		mCmdLine = lpCmdLine;
	}
}
