#pragma once
#include "Util.h"
#include<math.h>
#include "D3D12Viewport.h"

class FWindow : FNoncopyable
{
	FWindow() = delete;

public:
	LRESULT WndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);
	FWindow(DWORD _windowStyleEx, DWORD _windowStyle, UINT _x, UINT _y, UINT _width, UINT _height, const std::wstring & _windowName);
	__forceinline HWND GetHwnd()const { return mHwnd; }
	__forceinline FD3D12Viewport * GetViewport() { return mViewport; }
	bool IsClosed() { return mHwnd == nullptr; }

protected:
	void Close();

private:
	HWND mHwnd;
	FD3D12Viewport * mViewport;
	DWORD mWindowStyle;
	DWORD mWindowStyleEx;
	UINT mWidth;
	UINT mHeight;
	std::wstring mWindowName;
};
