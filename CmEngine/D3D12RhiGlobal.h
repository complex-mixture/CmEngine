#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <array>

class FD3D12Fence;

extern IDXGIFactory * GFactory;
extern IDXGIAdapter * GAdapter;
extern ID3D12Device * GDevice;
extern ID3D12GraphicsCommandList * GCommandList;
extern ID3D12CommandQueue * GCommandQueue;
extern FD3D12Fence * GFence;
extern std::array<ID3D12CommandAllocator*, 3> GAllocators;
extern std::vector<IDXGIAdapter*> GAdapters;
