#pragma once
#include <DirectXMath.h>
#include "Actor.h"
#include "Util.h"

using namespace DirectX;

class ACameraActor : public AActor
{
	SpecificEntityType(EEntityType::Camera)

public:
	float GetFov()const { return mFov; }
	float GetNearClipDistance()const { return mNearClipDistance; }
	float GetFarClipDistance()const { return mFarClipDistance; }

	void SetFov(float _fov) { mFov = _fov; }
	void SetNearClipDistance(float _nearClipDistance) { mNearClipDistance = _nearClipDistance; }
	void SetFarClipDistance(float _farClipDistance) { mFarClipDistance = _farClipDistance; }

private:
	float mFov = PI / 2.f;
	float mNearClipDistance = 1.f;
	float mFarClipDistance = 10000.f;
};
