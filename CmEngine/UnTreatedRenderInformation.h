#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Canvas.h"
#include "SkyBoxActor.h"
#include "StaticMeshActor.h"
#include "DirectionLightActor.h"
#include "PointLightActor.h"
#include "SpotLightActor.h"
#include "CameraActor.h"

class FCpuRenderFrameResource;
class UWorld;
class FCanvas;
class UMesh;
class UMaterials;

struct FUntreatedCamera
{
	float mFov;
	float mNearClipDistance = 1.f;
	float mFarClipDistance = 10000.f;

	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mRotation;

	FUntreatedCamera() = default;
	FUntreatedCamera(const ACameraActor * _actor)
	{
		mFov = _actor->GetFov();
		mNearClipDistance = _actor->GetNearClipDistance();
		mFarClipDistance = _actor->GetFarClipDistance();

		mPosition = _actor->GetPosition();
		mRotation = _actor->GetRotation();
	}
};

struct FUntreatedStaticMesh
{
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mRotation;
	DirectX::XMFLOAT3 mScale;

	UMesh * mStaticMesh;
	UMaterials * mMaterials;
	std::vector<FShaderParameter> mShaderParameters;

	FUntreatedStaticMesh() = default;
	FUntreatedStaticMesh(const AStaticMeshActor * _actor)
	{
		mPosition = _actor->GetPosition();
		mRotation = _actor->GetRotation();
		mScale = _actor->GetScale();

		mStaticMesh = _actor->GetStaticMesh();
		mMaterials = _actor->GetMaterials();
		mShaderParameters = _actor->GetShaderParameters();
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
	float mFallOffBegin;
	float mFallOffEnd;

	FUntreatedPointLight() = default;
	FUntreatedPointLight(const APointLightActor * _actor)
	{
		mColor = _actor->GetColor();
		mPosition = _actor->GetPosition();
		mIntensity = _actor->GetIntensity();
		mFallOffBegin = _actor->GetFallOffBegin();
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

struct FUntreatedSkyBox
{
	UMesh * mStaticMesh;
	UMaterials * mMaterials;
	std::vector<FShaderParameter> mShaderParameters;

	FUntreatedSkyBox() = default;
	FUntreatedSkyBox(const ASkyBoxActor * _actor)
	{
		mStaticMesh = _actor->GetStaticMesh();
		mMaterials = _actor->GetMaterials();
		mShaderParameters = _actor->GetShaderParameters();
	}
};

struct FUntreatedRenderInformation
{
	FCanvas mCanvas;
	float mDeltaTime;
	float mTotalTime;

	FUntreatedCamera mUntreatedCamera;
	FUntreatedSkyBox mUntreatedSkyBox;
	std::vector<FUntreatedStaticMesh> mUntreatedStaticMeshs;
	std::vector<FUntreatedDirectionLight> mUntreatedDirectionLights;
	std::vector<FUntreatedPointLight> mUntreatedPointLights;
	std::vector<FUntreatedSpotLight> mUntreatedSpotLights;

	FUntreatedRenderInformation() = default;
	FUntreatedRenderInformation(FCanvas _canvas, UWorld* _world, FCpuRenderFrameResource * _cpuRenderFrameResource);
};
