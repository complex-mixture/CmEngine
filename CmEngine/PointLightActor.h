#pragma once
#include "LightActor.h"
class APointLightActor : public ALightActor
{
public:
	__forceinline void SetFallOffBegin(float _fallOffBegin) { mFallOffBegin = _fallOffBegin; }
	__forceinline void SetFallOffEnd(float _fallOffEnd) { mFallOffEnd = _fallOffEnd; }

	__forceinline float GetFallOffBegin()const { return mFallOffBegin; }
	__forceinline float GetFallOffEnd()const { return mFallOffEnd; }

private:
	float mFallOffBegin;
	float mFallOffEnd;
};
