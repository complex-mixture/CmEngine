#include "PreTreatedRenderInformation.h"
#include "UnTreatedRenderInformation.h"
#include <DirectXCollision.h>

FPreTreatedSkyBox::FPreTreatedSkyBox(const FUntreatedSkyBox & _utskybox)
{
	mIndicesCount = _utskybox.mStaticMesh->GetIndicesCount();
	mVertexBufferView = _utskybox.mStaticMesh->GetVertexBufferView();
	mIndexBufferView = _utskybox.mStaticMesh->GetIndexBufferView();

	mRootSignature = _utskybox.mMaterials->GetRootSignature();
	mPipelineState = _utskybox.mMaterials->GetPipelineState();

	mShaderParameters = _utskybox.mShaderParameters;
}

FPreTreatedStaticMesh::FPreTreatedStaticMesh(const FUntreatedStaticMesh & _utsm)
{
	DirectX::XMMATRIX worldMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&_utsm.mScale));
	worldMatrix *= XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&_utsm.mRotation));
	worldMatrix *= XMMatrixTranslationFromVector(XMLoadFloat3(&_utsm.mPosition));

	DirectX::XMMATRIX worldMatrixInv = XMMatrixInverse(&XMMatrixDeterminant(worldMatrix), worldMatrix);
	XMStoreFloat4x4(&mObjCb.mWroldMatrix, XMMatrixTranspose(worldMatrix));
	XMStoreFloat4x4(&mObjCb.mWroldMatrixInv, XMMatrixTranspose(worldMatrixInv));

	mIndicesCount = _utsm.mStaticMesh->GetIndicesCount();
	mVertexBufferView = _utsm.mStaticMesh->GetVertexBufferView();
	mIndexBufferView = _utsm.mStaticMesh->GetIndexBufferView();

	mRootSignature = _utsm.mMaterials->GetRootSignature();
	mPipelineState = _utsm.mMaterials->GetPipelineState();

	mShaderParameters = _utsm.mShaderParameters;
}

FPreTreatedRenderInformation::FPreTreatedRenderInformation(const FUntreatedRenderInformation & _utri, FGpuRenderFrameResource * _gpuRenderFrameResource)
{
	if (_utri.mCanvas.mViewport == nullptr)
	{
		mRenderTarget = _utri.mCanvas.mRendertarget;
		mRenderTargetView = _utri.mCanvas.mRendertargetView;
	}
	else
	{
		mRenderTarget = _utri.mCanvas.mViewport->GetCurrentBackBuffer();
		mRenderTargetView = _utri.mCanvas.mViewport->GetCurrentBackBufferView().mCpuHandle;
	}

	mD3DViewport = _utri.mCanvas.mD3D12Viewport;
	mScissorRect = _utri.mCanvas.mScissorRect;

	XMVECTOR eyePosition = XMLoadFloat3(&_utri.mUntreatedCamera.mPosition);
	XMVECTOR forwardDirection = XMVector3TransformNormal(XMVectorSet(1.f, 0.f, 0.f, 1.f), XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&_utri.mUntreatedCamera.mRotation)));
	XMVECTOR upDirection = XMVector3TransformNormal(XMVectorSet(0.f, 0.f, 1.f, 1.f), XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&_utri.mUntreatedCamera.mRotation)));


	DirectX::XMMATRIX viewMatrix = XMMatrixLookToLH(eyePosition, forwardDirection, upDirection);
	DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovLH(_utri.mUntreatedCamera.mFov, _utri.mCanvas.GetAspectRatio(), _utri.mUntreatedCamera.mNearClipDistance, _utri.mUntreatedCamera.mFarClipDistance);
	DirectX::XMMATRIX viewProjMatrix = viewMatrix * projMatrix;

	DirectX::XMMATRIX viewMatrixInv = XMMatrixInverse(&XMMatrixDeterminant(viewMatrix), viewMatrix);
	DirectX::XMMATRIX projMatrixInv = XMMatrixInverse(&XMMatrixDeterminant(projMatrix), projMatrix);
	DirectX::XMMATRIX viewProjMatrixInv = XMMatrixInverse(&XMMatrixDeterminant(viewProjMatrix), viewProjMatrix);

	XMStoreFloat4x4(&mPassCb.mViewMatrix, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&mPassCb.mVireMatrixInv, XMMatrixTranspose(viewMatrixInv));
	XMStoreFloat4x4(&mPassCb.mProjMatrix, XMMatrixTranspose(projMatrix));
	XMStoreFloat4x4(&mPassCb.mProjMatrixInv, XMMatrixTranspose(projMatrixInv));
	XMStoreFloat4x4(&mPassCb.mViewProjMatrix, XMMatrixTranspose(viewProjMatrix));
	XMStoreFloat4x4(&mPassCb.mViewProjMatrixInv, XMMatrixTranspose(viewProjMatrixInv));

	mPassCb.mRenderTargetSize = XMFLOAT2(mD3DViewport.Width, mD3DViewport.Height);
	mPassCb.mRenderTargetSizeInv = XMFLOAT2(1.f / mD3DViewport.Width, 1.f / mD3DViewport.Height);

	mPassCb.mEyePosition = _utri.mUntreatedCamera.mPosition;

	mPassCb.mDeltaTime = _utri.mDeltaTime;
	mPassCb.mTotalTime = _utri.mTotalTime;

	mPassCb.mAmbientColor = XMFLOAT3(0.2f, 0.2f, 0.2f);

	uint32_t lightIndex = 0;

	for (auto const & _ele : _utri.mUntreatedDirectionLights)
	{
		mPassCb.mLights[lightIndex].mColor = _ele.mColor;
		mPassCb.mLights[lightIndex].mIntensity = _ele.mIntensity;
		mPassCb.mLights[lightIndex].mDirection = _ele.mDirection;
		lightIndex++;
	}

	mPassCb.mPointLightIndexStart = lightIndex;
	for (auto const & _ele : _utri.mUntreatedPointLights)
	{
		mPassCb.mLights[lightIndex].mColor = _ele.mColor;
		mPassCb.mLights[lightIndex].mIntensity = _ele.mIntensity;
		mPassCb.mLights[lightIndex].mPosition = _ele.mPosition;
		mPassCb.mLights[lightIndex].mFallOffBegin = _ele.mFallOffBegin;
		mPassCb.mLights[lightIndex].mFalloffEnd = _ele.mFallOffEnd;
		mPassCb.mLights[lightIndex].mFallOffBeginSqr = _ele.mFallOffBegin * _ele.mFallOffBegin;
		mPassCb.mLights[lightIndex].mFallOffEndSqr = _ele.mFallOffEnd * _ele.mFallOffEnd;
		lightIndex++;
	}

	mPassCb.mSpotLightIndexStart = lightIndex;
	for (auto const & _ele : _utri.mUntreatedSpotLights)
	{
		mPassCb.mLights[lightIndex].mColor = _ele.mColor;
		mPassCb.mLights[lightIndex].mIntensity = _ele.mIntensity;
		mPassCb.mLights[lightIndex].mPosition = _ele.mPosition;
		mPassCb.mLights[lightIndex].mDirection = _ele.mDirection;
		mPassCb.mLights[lightIndex].mFallOffBegin = _ele.mFallOffBegin;
		mPassCb.mLights[lightIndex].mFalloffEnd = _ele.mFallOffEnd;
		mPassCb.mLights[lightIndex].mFallOffBeginSqr = _ele.mFallOffBegin * _ele.mFallOffBegin;
		mPassCb.mLights[lightIndex].mFallOffEndSqr = _ele.mFallOffEnd * _ele.mFallOffEnd;
		mPassCb.mLights[lightIndex].mCosHalfInnerConeAngle = cos(_ele.mInnerConeAngle / 2.f);
		mPassCb.mLights[lightIndex].mCosHalfOuterConeAngle = cos(_ele.mOuterConeAngle / 2.f);
		lightIndex++;
	}

	new(&mPtSkyBox) FPreTreatedSkyBox(_utri.mUntreatedSkyBox);

	for (size_t i = 0, j = 0; i != _utri.mUntreatedStaticMeshs.size(); ++i)
	{
		DirectX::XMVECTOR cameraForward = XMVector3Normalize(forwardDirection);
		DirectX::XMVECTOR smPosition = XMLoadFloat3(&_utri.mUntreatedStaticMeshs[i].mPosition);
		DirectX::XMVECTOR toSm = XMVector3Normalize(XMVectorSubtract(smPosition, eyePosition));

		//if(XMVectorGetX(XMVector3Dot(cameraForward, toSm)) < 0.f) continue;

		mPtStaticMeshs.push_back(FPreTreatedStaticMesh(_utri.mUntreatedStaticMeshs[i]));

		for (uint32_t k = 0; k != lightIndex; ++k)
		{
			mPtStaticMeshs[j].mObjCb.mRelatedLightIndeices[k] = k;
		}
		mPtStaticMeshs[j].mObjCb.mRelatedLightCount = lightIndex;
		mPtStaticMeshs[j].mObjCb.mRelatedDirectionLightCount = _utri.mUntreatedDirectionLights.size();
		mPtStaticMeshs[j].mObjCb.mRelatedPointLightCount = _utri.mUntreatedPointLights.size();
		mPtStaticMeshs[j].mObjCb.mRelatedSpotLightCount = _utri.mUntreatedSpotLights.size();
		++j;
	}
}
