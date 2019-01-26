#include "BaseRenderer.h"
#include <DirectXColors.h>
#include "ShaderParameter.h"
#include "DescriptorHandleManager.h"
#include "RendererUtil.h"

void FBaseRenderer::RenderScene(FTreatedRenderInformation * _renderInformation)
{
	auto * ri = _renderInformation;
	GCommandList->RSSetViewports(1, &ri->mD3DViewport);
	GCommandList->RSSetScissorRects(1, &ri->mScissorRect);
	GCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(ri->mRenderTarget, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET));
	GCommandList->ClearDepthStencilView(ri->mDepthStencilView, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.f, 0, 1, &ri->mScissorRect);
	GCommandList->ClearRenderTargetView(ri->mRenderTargetView, DirectX::Colors::Black, 1, &ri->mScissorRect);
	GCommandList->OMSetRenderTargets(1, &ri->mRenderTargetView, true, &ri->mDepthStencilView);
	auto a = FDescriptorHandleManager<D3D12_SHADER_RESOURCE_VIEW_DESC>::Get().GetDescriptorHeap();
	GCommandList->SetDescriptorHeaps(1, &a);

	GCommandList->SetGraphicsRootSignature(ri->mTreatedSkyBox.mRootSignature);
	GCommandList->SetPipelineState(ri->mTreatedSkyBox.mPipelineState);
	GCommandList->SetGraphicsRootConstantBufferView(0, ri->mMainPassCb->GetElementGpuAddress(0));
	SetShaderParameters(ri->mTreatedSkyBox.mShaderParameters, 1);
	GCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GCommandList->IASetIndexBuffer(&ri->mTreatedSkyBox.mIndexBufferView);
	GCommandList->IASetVertexBuffers(0, 1, &ri->mTreatedSkyBox.mVertexBufferView);
	GCommandList->DrawIndexedInstanced(ri->mTreatedSkyBox.mIndicesCount, 1, 0, 0, 0);

	for (auto & _ele : ri->mTreatedStaticMeshs)
	{
		GCommandList->SetGraphicsRootSignature(_ele.mRootSignature);
		GCommandList->SetPipelineState(_ele.mPipelineState);
		GCommandList->SetGraphicsRootConstantBufferView(0, ri->mMainPassCb->GetElementGpuAddress(0));
		GCommandList->SetGraphicsRootConstantBufferView(1, ri->mObjCb->GetElementGpuAddress(_ele.mIndexInObjCb));
		SetShaderParameters(_ele.mShaderParameters, 2);
		GCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		GCommandList->IASetIndexBuffer(&_ele.mIndexBufferView);
		GCommandList->IASetVertexBuffers(0, 1, &_ele.mVertexBufferView);
		GCommandList->DrawIndexedInstanced(_ele.mIndicesCount, 1, 0, 0, 0);
	}

	GCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(ri->mRenderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COMMON));
}

void FBaseRenderer::Release()
{
	delete this;
}
