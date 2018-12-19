#pragma once
#include <DirectXMath.h>
#include <vector>
#include "StaticMeshActor.h"
#include "DirectionLightActor.h"
#include "PointLightActor.h"
#include "SpotLightActor.h"
#include "Canvas.h"

class FCpuRenderFrameResource;
class UWorld;
class FCanvas;
class UStaticMesh;
class UMaterials;

struct FUntreatedRenderStaticMesh
{
	DirectX::XMMATRIX mWorldMatrix;
	UStaticMesh * mStaticMesh;
	UMaterials * mMaterials;

	FUntreatedRenderStaticMesh() = default;
	FUntreatedRenderStaticMesh(const AStaticMeshActor * _actor)
	{
		mWorldMatrix = _actor->GetWorldMatrix();
		mStaticMesh = _actor->GetStaticMesh();
		mMaterials = _actor->GetMaterials();
	}
};

struct FUntreatedDirectionLight
{
	DirectX::XMFLOAT3 mColor;
	DirectX::XMFLOAT3 mDirection;
	float mIntensity;

	FUntreatedDirectionLight() = default;
	FUntreatedDirectionLight(const ADirectionLightActor * _actor)
	{
		mColor = _actor->GetColor();
		mDirection = _actor->GetForwardDirection();
		mIntensity = _actor->GetIntensity();
	}
};

struct FUntreatedPointLight
{
	DirectX::XMFLOAT3 mColor;
	DirectX::XMFLOAT3 mPosition;
	float mIntensity;
	float mFallOffStart;
	float mFallOffEnd;

	FUntreatedPointLight() = default;
	FUntreatedPointLight(const APointLightActor * _actor)
	{
		mColor = _actor->GetColor();
		mPosition = _actor->GetPosition();
		mIntensity = _actor->GetIntensity();
		mFallOffStart = _actor->GetFallOffStart();
		mFallOffEnd = _actor->GetFallOffEnd();
	}
};

struct FUntreatedSpotLight : FUntreatedPointLight
{
	DirectX::XMFLOAT3 mDirection;
	float mInnerConeAngle;
	float mOuterConeAngle;

	FUntreatedSpotLight() = default;
	FUntreatedSpotLight(const ASpotLightActor * _actor) :FUntreatedPointLight(_actor)
	{
		mDirection = _actor->GetForwardDirection();
		mInnerConeAngle = _actor->GetInnerConeAngle();
		mOuterConeAngle = _actor->GetOuterConeAngle();
	}
};

struct FUntreatedRenderInformation
{
	FCanvas mCanvas;
	DirectX::XMMATRIX mViewMatrix;
	DirectX::XMFLOAT3 mEyePosition;
	float mDeltaTime;
	float mTotalTime;
	std::vector<FUntreatedRenderStaticMesh> mUntreatedRenderStaticMeshs;
	std::vector<FUntreatedDirectionLight> mUntreatedDirectionLights;
	std::vector<FUntreatedPointLight> mUntreatedPointLights;
	std::vector<FUntreatedSpotLight> mUntreatedSpotLights;

	FUntreatedRenderInformation() = default;
	FUntreatedRenderInformation(FCanvas _canvas, UWorld* _world, FCpuRenderFrameResource * _cpuRenderFrameResource);
};