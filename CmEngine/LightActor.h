#pragma once
#include "Actor.h"

class ALightActor : public AActor
{
public:
	__forceinline void SetIntensity(float _intensity) { mIntensity = _intensity; }
	__forceinline void SetColor(XMFLOAT3 _color) { mColor = _color; }

	__forceinline float	 GetIntensity()const { return mIntensity; }
	__forceinline XMFLOAT3 GetColor()const { return mColor; }

private:
	float mIntensity;
	XMFLOAT3 mColor;
};
