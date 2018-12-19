#include "D3D12RhiModule.h"
#include "D3D12RhiGlobal.h"
#include "ModuleRegister.h"
#include "d3dx12.h"
#include <vector>
#include <dxgidebug.h>
#include <set>
#include "D3D12Viewport.h"
#include "D3D12Fence.h"
#include "D3D12RhiUtil.h"
#include <windows.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#ifdef DEBUG
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif // DEBUG
#include "DescriptorHandleManager.h"
#include <d3d12.h>

IDXGIFactory * GFactory = nullptr;
IDXGIAdapter* GAdapter = nullptr;
ID3D12Device * GDevice = nullptr;
ID3D12GraphicsCommandList * GCommandList = nullptr;
ID3D12CommandQueue * GCommandQueue = nullptr;
FD3D12Fence * GFence = nullptr;
std::array<ID3D12CommandAllocator*, 3> GAllocators;
std::vector<IDXGIAdapter*> GAdapters;

class FD3D12RhiModuleImpl : public ID3D12RhiModuleInterface
{
	std::set<FD3D12Viewport*> mSwapChains;

public:
	void Init();
	void Clear();
	FD3D12Viewport * CreateViewport(HWND _windowHandle, uint32_t _sizeX, uint32_t _sizeY, bool _isFullscreen, DXGI_FORMAT _pixelFormat);
	uint32_t Present();
	void ReportLiveObject();
};

void FD3D12RhiModuleImpl::Init()
{
#ifdef DEBUG
	ID3D12Debug * debugController = nullptr;
	VerifyD3D12Result(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	debugController->EnableDebugLayer();
	//debugController->Release();
#endif


	VerifyD3D12Result(CreateDXGIFactory(IID_PPV_ARGS(&GFactory)));

	{
		UINT i = 0;
		IDXGIAdapter * _adapter = nullptr;
		while (GFactory->EnumAdapters(i++, &_adapter) != DXGI_ERROR_NOT_FOUND)
			GAdapters.push_back(_adapter);
		if (GAdapters.size() == 0)
		{
			DebugMessageBoxW(L"D3D12", L"没有可用的显示适配器");
		}
		GAdapter = GAdapters[0];
	}
	VerifyD3D12Result(D3D12CreateDevice(GAdapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&GDevice)));

	for (int i = 0; i != GAllocators.size(); ++i)
		VerifyD3D12Result(GDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&GAllocators[i])));
	VerifyD3D12Result(GDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, GAllocators[0], nullptr, IID_PPV_ARGS(&GCommandList)));

	{
		D3D12_COMMAND_QUEUE_DESC desc;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;
		desc.Priority = 0;
		desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		VerifyD3D12Result(GDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&GCommandQueue)));
	}
	FDescriptorHandleManager<D3D12_RENDER_TARGET_VIEW_DESC>::Get().Init(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 256);
	FDescriptorHandleManager<D3D12_DEPTH_STENCIL_VIEW_DESC>::Get().Init(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 256);
	FDescriptorHandleManager<D3D12_CONSTANT_BUFFER_VIEW_DESC>::Get().Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 2048);
	FDescriptorHandleManager<D3D12_SHADER_RESOURCE_VIEW_DESC>::Get().Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1024);
	FDescriptorHandleManager<D3D12_UNORDERED_ACCESS_VIEW_DESC>::Get().Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1024);

	GCommandList->Close();
	GFence = new FD3D12Fence(0);
	Assert(GFactory && GAdapter && GDevice && GCommandList && GCommandQueue);
}

void FD3D12RhiModuleImpl::Clear()
{
	FDescriptorHandleManager<D3D12_RENDER_TARGET_VIEW_DESC>::Get().Clear();
	FDescriptorHandleManager<D3D12_DEPTH_STENCIL_VIEW_DESC>::Get().Clear();
	FDescriptorHandleManager<D3D12_CONSTANT_BUFFER_VIEW_DESC>::Get().Clear();
	FDescriptorHandleManager<D3D12_SHADER_RESOURCE_VIEW_DESC>::Get().Clear();
	FDescriptorHandleManager<D3D12_UNORDERED_ACCESS_VIEW_DESC>::Get().Clear();

	GFactory->Release();
	GDevice->Release();
	GCommandList->Release();
	GCommandQueue->Release();

	delete GFence;
	for (int i = 0; i != 3; ++i)
	{
		GAllocators[i]->Release();
		GAllocators[i] = nullptr;
	}

	for (auto _ele : GAdapters)
		_ele->Release();

	GFactory = nullptr;
	GAdapter = nullptr;
	GDevice = nullptr;
	GCommandList = nullptr;
	GCommandQueue = nullptr;
	GFence = nullptr;
	GAdapters.clear();

	ReportLiveObject();
}

FD3D12Viewport * FD3D12RhiModuleImpl::CreateViewport(HWND _windowHandle, uint32_t _sizeX, uint32_t _sizeY, bool _isFullscreen, DXGI_FORMAT _pixelFormat)
{
	FD3D12Viewport * newSwapChain = new FD3D12Viewport(_windowHandle, _sizeX, _sizeY, _isFullscreen, _pixelFormat);
	mSwapChains.insert(newSwapChain);
	return newSwapChain;
}

uint32_t FD3D12RhiModuleImpl::Present()
{
	uint32_t count = 0;
	for (auto _ele : mSwapChains)
		count += _ele->Present();

	return count;
}

void FD3D12RhiModuleImpl::ReportLiveObject()
{
#ifdef DEBUG
	LogA("ReportLiveObject : \n");
	IDXGIDebug * debug = nullptr;
	HMODULE handle = LoadLibrary(L"DXGIDebug.dll");
	using DXGIGetDebugInterfaceFuncType = HRESULT(WINAPI *)(REFIID, void **);
	DXGIGetDebugInterfaceFuncType DXGIGetDebugInterfaceFunc = reinterpret_cast<DXGIGetDebugInterfaceFuncType>(GetProcAddress(handle, "DXGIGetDebugInterface"));
	DXGIGetDebugInterfaceFunc(IID_PPV_ARGS(&debug));
	debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
	debug->Release();
	FreeLibrary(handle);
	LogA("ReportLiveObjectEnd\n\n");

#endif // DEBUG
}

RegistDefaultModule(FD3D12RhiModuleImpl, D3D12Rhi);
