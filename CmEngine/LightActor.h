#pragma once
#include "Actor.h"

enum class ELightType
{
	Unknow = -1,
	Direction = 0,
	Point,
	Spot,
	Num,
	NumBits = 2
};

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