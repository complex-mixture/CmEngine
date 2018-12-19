#include "Window.h"
#include "WindowManager.h"
#include "ModuleManager.h"
#include "D3D12RhiModule.h"
#include "RenderModule.h"

LRESULT FWindow::WndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_msg)
	{
	case WM_CLOSE:
		Close();
		return 0;
	}
	return DefWindowProcW(_hwnd, _msg, _wParam, _lParam);
}

FWindow::FWindow(DWORD _windowStyleEx, DWORD _windowStyle, UINT _x, UINT _y, UINT _width, UINT _height, const std::wstring & _windowName)
	:mWindowStyleEx(_windowStyleEx),
	mWindowStyle(_windowStyle),
	mWindowName(_windowName)
{
	mHwnd = CreateWindowExW(_windowStyleEx, L"CMWindow", _windowName.c_str(), _windowStyle, _x, _y, _width, _height, NULL, NULL, FWindowManager::Get().GetInstance(), NULL);
	{
		RECT rect;
		GetClientRect(mHwnd, &rect);
		mWidth = rect.right;
		mHeight = rect.bottom;
	}
	mViewport = GetD3D12RhiModule()->CreateViewport(mHwnd, mWidth, mHeight, false, DXGI_FORMAT_R8G8B8A8_UNORM);
	FWindowManager::Get().AddHandleMap(mHwnd, this);
	ShowWindow(mHwnd, SW_SHOW);
}

void FWindow::Close()
{
	FWindowManager::Get().EraseHandleMap(mHwnd);
	DestroyWindow(mHwnd);
	GetRenderModule()->AddTaskOnRenderThreadFlush([_viewport = mViewport]() {delete _viewport; });
	mHwnd = nullptr;
	mViewport = nullptr;
}
