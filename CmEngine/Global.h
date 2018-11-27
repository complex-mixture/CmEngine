#pragma once
#include <thread>

class FEngineLoop;
class FEngine;

extern FEngineLoop GEngineLoop;
extern FEngine * GEngine;

extern uint64_t GFrameCount;

extern std::thread::id GGameThreadId;
extern std::thread::id GRenderThreadId;

__forceinline bool IsInGaneThread()
{
	return std::this_thread::get_id() == GGameThreadId;
}

__forceinline bool IsInRenderThread()
{
	return std::this_thread::get_id() == GRenderThreadId;
}
