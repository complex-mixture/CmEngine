#include "PreTreatedRenderInformation.h"
#include "UnTreatedRenderInformation.h"

FPreTreatedRenderStaticMesh::FPreTreatedRenderStaticMesh(const FUntreatedRenderStaticMesh & _utsm)
{
	DirectX::XMMATRIX worldMatrixInv = XMMatrixInverse(&XMMatrixDeterminant(_utsm.mWorldMatrix), _utsm.mWorldMatrix);
	XMStoreFloat4x4(&mObjCb.mWroldMatrix, XMMatrixTranspose(_utsm.mWorldMatrix));
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

	DirectX::XMMATRIX viewMatrix = _utri.mViewMatrix;
	DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovLH(PI / 2.f, _utri.mCanvas.GetAspectRatio(), 1.f, 1000.f);
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

	mPassCb.mEyePosition = _utri.mEyePosition;

	mPassCb.mDeltaTime = _utri.mDeltaTime;
	mPassCb.mTotalTime = _utri.mTotalTime;

	mPassCb.mAmbientColor = XMFLOAT3(0.2f, 0.2f, 0.2f);

	uint32_t lightIndex = 0;

	for (auto const & _ele : _utri.mUntreatedDirectionLights)
	{
		mPassCb.mLights[lightIndex].mColor = _ele.mColor;
		mPassCb.mLights[lightIndex].mDirection = _ele.mDirection;
		mPassCb.mLights[lightIndex].mIntensity = _ele.mIntensity;
		lightIndex++;
	}
	mPassCb.mDirectionLightIndexEnd = lightIndex - 1;

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
	mPassCb.mPointLightIndexEnd = lightIndex - 1;

	for (auto const & _ele : _utri.mUntreatedSpotLights)
	{
		mPassCb.mLights[lightIndex].mColor = _ele.mColor;
		mPassCb.mLights[lightIndex].mIntensity = _ele.mIntensity;
		mPassCb.mLights[lightIndex].mPosition = _ele.mPosition;
		mPassCb.mLights[lightIndex].mFallOffBegin = _ele.mFallOffBegin;
		mPassCb.mLights[lightIndex].mFalloffEnd = _ele.mFallOffEnd;
		mPassCb.mLights[lightIndex].mFallOffBeginSqr = _ele.mFallOffBegin * _ele.mFallOffBegin;
		mPassCb.mLights[lightIndex].mFallOffEndSqr = _ele.mFallOffEnd * _ele.mFallOffEnd;
		mPassCb.mLights[lightIndex].mCosHalfInnerConeAngle = cos(_ele.mInnerConeAngle / 2.f);
		mPassCb.mLights[lightIndex].mCosHalfInnerConeAngle = cos(_ele.mOuterConeAngle / 2.f);
		lightIndex++;
	}

	mPreUreatedRenderStaticMeshs.resize(_utri.mUntreatedRenderStaticMeshs.size());
	for (size_t i = 0; i != _utri.mUntreatedRenderStaticMeshs.size(); ++i)
	{
		new(&mPreUreatedRenderStaticMeshs[i]) FPreTreatedRenderStaticMesh(_utri.mUntreatedRenderStaticMeshs[i]);
		uint32_t relatedLightsCount = 0;
		for (uint32_t j = 0; j != lightIndex; ++j)
		{
			mPreUreatedRenderStaticMeshs[i].mObjCb.mRelatedLightIndeices[j] = j;
		}
		mPreUreatedRenderStaticMeshs[i].mObjCb.mRelatedLightCount = lightIndex;
		//mPreUreatedRenderStaticMeshs[i].mObjCb.mRelatedDirectionLightCount = _utri.mUntreatedDirectionLights.size();
		//mPreUreatedRenderStaticMeshs[i].mObjCb.mRelatedPointLightCount = _utri.mUntreatedPointLights.size();
		//mPreUreatedRenderStaticMeshs[i].mObjCb.mRelatedSpotLightCount = _utri.mUntreatedSpotLights.size();
	}
}
