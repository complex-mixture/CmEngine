#pragma once
#include "ModuleInterface.h"
#include "ModuleManager.h"
#include <dxgi.h>
#include <stdint.h>

class FD3D12Viewport;

__interface ID3D12RhiModuleInterface : IModuleInterface
{
	FD3D12Viewport * CreateViewport(HWND _windowHandle, uint32_t _sizeX, uint32_t _sizeY, bool _isFullscreen, DXGI_FORMAT _pixelFormat);
	uint32_t Present();
	void ReportLiveObject();
};

inline ID3D12RhiModuleInterface * GetD3D12RhiModule()
{
	return FModuleManager::Get().GetModule<ID3D12RhiModuleInterface>(L"D3D12Rhi");
}
