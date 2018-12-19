#pragma once
#include "LightActor.h"
class APointLightActor : public ALightActor
{
public:
	__forceinline void SetFallOffStart(float _fallOffStart) { mFallOffStart = _fallOffStart; }
	__forceinline void SetFallOffEnd(float _fallOffEnd) { mFallOffEnd = _fallOffEnd; }

	__forceinline float GetFallOffStart()const { return mFallOffStart; }
	__forceinline float GetFallOffEnd()const { return mFallOffEnd; }

private:
	float mFallOffStart;
	float mFallOffEnd;
};