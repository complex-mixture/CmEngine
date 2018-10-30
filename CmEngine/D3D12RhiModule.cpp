#include "D3D12RhiModule.h"
#include "D3D12RhiGlobal.h"
#include "ModuleRegister.h"
#include "d3dx12.h"
#include <vector>
#include <set>
#include "D3D12Viewport.h"
#include "D3D12Fence.h"
#include "D3D12RhiUtil.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

IDXGIFactory * GFactory = nullptr;
IDXGIAdapter* GAdapter = nullptr;
ID3D12Device * GDevice = nullptr;
ID3D12GraphicsCommandList * GCommandList = nullptr;
ID3D12CommandQueue * GCommandQueue = nullptr;
FD3D12Fence * GFence = nullptr;
std::vector<IDXGIAdapter*> GAdapters;

class FD3D12RhiInterface : public ID3D12RhiInterface
{
	std::set<FD3D12Viewport*> mSwapChains;
public:
	void Init();
	void Clear();
	FD3D12Viewport * CreateViewport(HWND _windowHandle, uint32_t _sizeX, uint32_t _sizeY, bool _isFullscreen, DXGI_FORMAT _pixelFormat);
	uint32_t Present();
};


void FD3D12RhiInterface::Init()
{
#ifdef DEBUG
	ID3D12Debug * debugController = nullptr;
	VerifyD3D12Result(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	debugController->EnableDebugLayer();
	debugController->Release();
#endif

	VerifyD3D12Result(CreateDXGIFactory(IID_PPV_ARGS(&GFactory)));

	{
		UINT i = 0;
		IDXGIAdapter * _adapter = nullptr;
		while (GFactory->EnumAdapters(i, &_adapter) != DXGI_ERROR_NOT_FOUND)
			GAdapters.push_back(_adapter);
		if (GAdapters.size() == 0)
			DebugMessageBoxW(L"D3D12", L"没有可用的显示适配器");
		GAdapter = GAdapters[0];
	}
	VerifyD3D12Result(D3D12CreateDevice(GAdapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&GDevice)));
	VerifyD3D12Result(GDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, nullptr, nullptr, IID_PPV_ARGS(&GCommandList)));
	GFence = new FD3D12Fence(0);

	{
		D3D12_COMMAND_QUEUE_DESC desc;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;
		desc.Priority = 0;
		desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		VerifyD3D12Result(GDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&GCommandQueue)));
	}

	Assert(GFactory && GAdapter && GDevice && GCommandList && GCommandQueue);
}

void FD3D12RhiInterface::Clear()
{
	GFactory->Release();
	GDevice->Release();
	GCommandList->Release();
	GCommandQueue->Release();
	for (auto _ele : GAdapters)
		_ele->Release();

	GFactory = nullptr;
	GAdapter = nullptr;
	GDevice = nullptr;
	GCommandList = nullptr;
	GCommandQueue = nullptr;
	GAdapters.clear();
}

FD3D12Viewport * FD3D12RhiInterface::CreateViewport(HWND _windowHandle, uint32_t _sizeX, uint32_t _sizeY, bool _isFullscreen, DXGI_FORMAT _pixelFormat)
{
	FD3D12Viewport * newSwapChain = new FD3D12Viewport(_windowHandle, _sizeX, _sizeY, _isFullscreen, _pixelFormat);
	mSwapChains.insert(newSwapChain);
	return newSwapChain;
}

uint32_t FD3D12RhiInterface::Present()
{
	uint32_t count = 0;
	for (auto _ele : mSwapChains)
		count += _ele->Present();

	return count;
}

RegistDefaultModule(FD3D12RhiInterface, D3D12Rhi);
