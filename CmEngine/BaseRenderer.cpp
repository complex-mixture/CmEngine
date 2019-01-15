#include "BaseRenderer.h"
#include <DirectXColors.h>
#include "ShaderParameter.h"
#include "DescriptorHandleManager.h"

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
	for (auto & _ele : _renderInformation->mTreatedRenderStaticMeshs)
	{
		GCommandList->SetGraphicsRootSignature(_ele.mRootSignature);
		GCommandList->SetPipelineState(_ele.mPipelineState);
		GCommandList->SetGraphicsRootConstantBufferView(0, ri->mMainPassCb->GetElementGpuAddress(0));
		GCommandList->SetGraphicsRootConstantBufferView(1, ri->mObjCb->GetElementGpuAddress(_ele.mIndexInObjCb));
		for (uint64_t i = 0; i != _ele.mShaderParameters.size(); ++i)
		{
			switch (_ele.mShaderParameters[i].mParameterType)
			{
			case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
				GCommandList->SetGraphicsRootDescriptorTable(i + 2, _ele.mShaderParameters[i].mParameterDescriptorTable);
				break;
			case D3D12_ROOT_PARAMETER_TYPE_CBV:
				GCommandList->SetComputeRootConstantBufferView(i + 2, _ele.mShaderParameters[i].mParameterConstantBufferView);
				break;
			case D3D12_ROOT_PARAMETER_TYPE_SRV:
				GCommandList->SetGraphicsRootShaderResourceView(i + 2, _ele.mShaderParameters[i].mParameterShaderResourcedView);
				break;
			case D3D12_ROOT_PARAMETER_TYPE_UAV:
				GCommandList->SetGraphicsRootUnorderedAccessView(i + 2, _ele.mShaderParameters[i].mParameterUnorderedAccessView);
				break;
			}
		}
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
