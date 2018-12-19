#pragma once
#include <DirectXMath.h>

struct FLight
{
	DirectX::XMFLOAT3 mPosition = DirectX::XMFLOAT3{ 0.f, 0.f, 0.f };
	float mIntensity = -1.f;
	DirectX::XMFLOAT3 mColor = DirectX::XMFLOAT3{ 0.f, 0.f, 0.f };
	float mFalloffEnd = -1.f;
	DirectX::XMFLOAT3 mDirection = DirectX::XMFLOAT3{ 0.f, 0.f, 0.f };
	float mFalloffStart = -1.f;
	float mInnerConeAngle = -1.f;
	float mOuterConeAngle = -1.f;
	float unused[2];
};

struct PassCb
{
	DirectX::XMFLOAT4X4 mViewMatrix;
	DirectX::XMFLOAT4X4 mVireMatrixInv;
	DirectX::XMFLOAT4X4 mProjMatrix;
	DirectX::XMFLOAT4X4 mProjMatrixInv;
	DirectX::XMFLOAT4X4 mViewProjMatrix;
	DirectX::XMFLOAT4X4 mViewProjMatrixInv;
	DirectX::XMFLOAT2 mRenderTargetSize;
	DirectX::XMFLOAT2 mRenderTargetSizeInv;
	DirectX::XMFLOAT3 mEyePosition;
	float mDeltaTime;
	DirectX::XMFLOAT3 mAmbientColor;
	float mTotalTime;
	uint32_t mPointLightIndexStart;
	uint32_t mSpotLightIndexStart;
	float unused[2];
	FLight mLights[255];
};

struct ObjectCb
{
	DirectX::XMFLOAT4X4 mWroldMatrix;
	DirectX::XMFLOAT4X4 mWroldMatrixInv;
	uint32_t mRelatedLightCount = 0;
	uint32_t mRelatedLightIndeices[255] = {};
};