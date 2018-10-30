#pragma once
#include <windows.h>

class FEngineLoop final
{
public:
	void PreInit(_In_ HINSTANCE hInstance, _In_ LPSTR lpCmdLine);
	void Init();
	void Tick();
	void Exit();

	bool RequestExit() { return false; }

	HINSTANCE mInstance;
	LPSTR mCmdLine;
};

extern FEngineLoop GEngineLoop;
