#pragma once
#include <DirectXMath.h>
#include "EEntityType.h"
#include "Util.h"
using namespace DirectX;
class AActor
{
public:
	static EEntityType GetStaticEntityType() { return EEntityType::Unknow; }
	virtual EEntityType GetEntityType()const { return GetStaticEntityType(); }

	void SetPosition(XMFLOAT3 _position) { mPosition = _position; }
	void SetRotation(XMFLOAT3 _rotation) { mRotation = _rotation; }
	void SetScale(XMFLOAT3 _scale) { mScale = _scale; }

	XMFLOAT3 GetForwardDirection()const
	{

		XMVECTOR _forwardDirection = XMVector4Transform(XMVectorSet(1.f, 0.f, 0.f, 1.f), XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&GetRotation())));
		XMFLOAT3 returned;
		XMStoreFloat3(&returned, _forwardDirection);
		return returned;
	}
	XMFLOAT3 GetPosition()const { return mPosition; }
	XMFLOAT3 GetRotation()const { return mRotation; }
	XMFLOAT3 GetScale()const { return mScale; }

	virtual void Tick(float _deltaTime) {}
	virtual void EndPlay() {}

#ifdef DEBUG
	void SetDebugName(const std::wstring &_debugName) { mDebugName = _debugName; }
#endif

private:
	XMFLOAT3 mPosition = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3 mRotation = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3 mScale = XMFLOAT3(1.f, 1.f, 1.f);

#ifdef DEBUG
	std::wstring mDebugName;
#endif
};
