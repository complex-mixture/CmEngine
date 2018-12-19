#include "BaseRenderer.h"
#include <DirectXColors.h>
#include "TextureManager.h"
#include "DescriptorHandleManager.h"

void FBaseRenderer::RenderScene(FTreatedRenderInformation * _renderInformation)
{
	auto * ri = _renderInformation;
	GCommandList->RSSetViewports(1, &ri->mD3DViewport);
	GCommandList->RSSetScissorRects(1, &ri->mScissorRect);
	GCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(ri->mRenderTarget, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET));
	GCommandList->ClearDepthStencilView(ri->mDepthStencilView, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.f, 0, 1, &ri->mScissorRect);
	GCommandList->ClearRenderTargetView(ri->mRenderTargetView, DirectX::Colors::BlueViolet, 1, &ri->mScissorRect);
	GCommandList->OMSetRenderTargets(1, &ri->mRenderTargetView, true, &ri->mDepthStencilView);
	auto a = FDescriptorHandleManager<D3D12_SHADER_RESOURCE_VIEW_DESC>::Get().GetDescriptorHeap();
	GCommandList->SetDescriptorHeaps(1, &a);
	for (auto & _ele : _renderInformation->mTreatedRenderStaticMeshs)
	{
		GCommandList->SetGraphicsRootSignature(_ele.mRootSignature);
		GCommandList->SetPipelineState(_ele.mPipelineState);
		GCommandList->SetGraphicsRootConstantBufferView(0, ri->mMainPassCb->GetElementGpuAddress(0));
		GCommandList->SetGraphicsRootConstantBufferView(1, ri->mObjCb->GetElementGpuAddress(_ele.mIndexInObjCb));
		GCommandList->SetGraphicsRootDescriptorTable(2, FTextureManager::Get().LoadMesh(L"bricks")->GetDescriptorHandle().mGpuHandle);
		GCommandList->SetGraphicsRootDescriptorTable(3, FTextureManager::Get().LoadMesh(L"default_nmap")->GetDescriptorHandle().mGpuHandle);
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
