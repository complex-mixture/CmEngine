#include "Util.h"
#include "EngineLoop.h"

float GetTotalTime()
{
	return GEngineLoop.GetTimer().TotalTime();
}

float GetDeltaTime()
{
	return GEngineLoop.GetTimer().DeltaTime();
}
