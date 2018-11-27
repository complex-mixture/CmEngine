#pragma once
#include <windows.h>
#include "GameTimer.h"

class FEngineLoop final
{
public:
	void PreInit(_In_ HINSTANCE hInstance, _In_ LPWSTR lpCmdLine);
	void Init();
	void Tick();
	void Exit();
	void RequestExit(bool _force);
	__forceinline bool ShouldExit()const { return bShouldExit; }

	__forceinline HINSTANCE GetInstance()const { return mInstance; }
	__forceinline LPWSTR GetCmdLine()const { return mCmdLine; }
private:
	bool bShouldExit = false;
	HINSTANCE mInstance;
	LPWSTR mCmdLine;
	FGameTimer mTimer;
};

extern FEngineLoop GEngineLoop;

void RequestExit(bool _force);
