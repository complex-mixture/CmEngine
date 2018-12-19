#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <set>
#include "D3D12Fence.h"
#include "Util.h"
#include "D3D12RhiGlobal.h"
#include "DescriptorHandleManager.h"

constexpr D3D12_RECT ZeroRect = D3D12_RECT{ 0, 0, 0, 0 };

class FD3D12ViewportChild;

class FD3D12Viewport : FNoncopyable
{
private:
	//only be called in construct function
	//be called after the construct function set all members which should be set
	void __Init();
public:
	static constexpr uint32_t MaxBackBufferCount = 10;
	static constexpr uint32_t DefaultBackBufferCount = 3;

	FD3D12Viewport(HWND _windowHandle, uint32_t _sizeX, uint32_t _sizeY, bool _isFullscreen, DXGI_FORMAT _pixelFormat);

	void Resize(uint32_t _sizeX, uint32_t _sizeY, bool _isFullscreen, DXGI_FORMAT _pixelFormat);

	ID3D12Resource * GetCurrentBackBuffer()const;
	FDescriptorHandle GetCurrentBackBufferView()const;
	uint32_t GetWeight()const { return mSizeX; }
	uint32_t GetHeight()const { return mSizeY; }
	void WaitForCompletion()const { GFence->WaitForCompletion(mLastUseFenceValue); }
	void SetLastUseFenceValue(uint64_t _value) { mLastUseFenceValue = max(mLastUseFenceValue, _value); }

	//is bShouldPresent is false, this function will do nothing
	uint32_t Present();

	__forceinline void SetShouldPresent(bool _shouldPresent) { bShouldPresent = _shouldPresent; }
	__forceinline bool GetShouldPresent()const { return bShouldPresent; }

	~FD3D12Viewport();

private:
	IDXGISwapChain * mSwapChain = nullptr;
	HWND mHwnd = nullptr;
	uint32_t mSizeX = 0;
	uint32_t mSizeY = 0;
	DXGI_FORMAT mPixelFormat = DXGI_FORMAT_UNKNOWN;

	uint32_t mBackBufferCount;
	uint32_t mCurrentBackBufferIndex = 0;
	ID3D12Resource * mBackBuffers[MaxBackBufferCount] = { 0 };
	FDescriptorHandle mBackBufferViews[MaxBackBufferCount] = {};
	uint64_t mLastUseFenceValue = 0;

	uint32_t bIsFullScreen : 1;
	uint32_t bShouldPresent : 1;

};
