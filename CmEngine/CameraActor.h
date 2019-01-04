#pragma once
#include <DirectXMath.h>
#include "Actor.h"

using namespace DirectX;
static XMVECTOR forwardDirection = XMVectorSet(1.f, 0.f, 0.f, 1.f);
static XMVECTOR upDirection = XMVectorSet(0.f, 0.f, 1.f, 1.f);

class ACameraActor : public AActor
{
public:
	XMMATRIX GetViewMatrix()const
	{
		XMVECTOR eyePosition = XMLoadFloat3(&GetPosition());
		XMVECTOR _forwardDirection = XMVector3TransformNormal(forwardDirection, XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&GetRotation())));
		return XMMatrixLookToLH(eyePosition, _forwardDirection, upDirection);
	}
};
