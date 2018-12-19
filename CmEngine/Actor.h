#pragma once
#include <DirectXMath.h>
using namespace DirectX;
class AActor
{
public:
	XMMATRIX GetWorldMatrix()const
	{
		XMMATRIX worldMatrix = XMMatrixIdentity();
		worldMatrix *= XMMatrixScalingFromVector(XMLoadFloat3(&mScale));
		worldMatrix *= XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&mRotation));
		worldMatrix *= XMMatrixTranslationFromVector(XMLoadFloat3(&mPosition));
		return worldMatrix;
	}

	void SetPosition(XMFLOAT3 _position) { mPosition = _position; }
	void SetRotation(XMFLOAT3 _rotation) { mRotation = _rotation; }
	void SetScale(XMFLOAT3 _scale) { mScale = _scale; }

	XMFLOAT3 GetForwardDirection()const
	{
		XMVECTOR eyePosition = XMLoadFloat3(&GetPosition());
		XMVECTOR _forwardDirection = XMVector3TransformNormal(XMVectorSet(1.f, 0.f, 0.f, 1.f), XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&GetRotation())));
		XMFLOAT3 returned;
		XMStoreFloat3(&returned, _forwardDirection);
		return returned;
	}
	XMFLOAT3 GetPosition()const { return mPosition; }
	XMFLOAT3 GetRotation()const { return mRotation; }
	XMFLOAT3 GetScale()const { return mScale; }

	virtual void Tick(float _deltaTime) {}

private:
	XMFLOAT3 mPosition = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3 mRotation = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3 mScale = XMFLOAT3(1.f, 1.f, 1.f);
};