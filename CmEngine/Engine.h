#pragma once
#include "Util.h"

class FWindow;
class UWorld;

class FEngine : FNoncopyable
{
private:

public:
	virtual	void Init();
	virtual void Tick(float _deltaTime);
	virtual void Exit();

private:
	UWorld * mMainWorld;
	FWindow * mClientWindow;
	FWindow * mClientWindow2;
};
extern FEngine * GEngine;
