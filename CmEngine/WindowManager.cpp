#include "Window.h"
#include <windows.h>
#include "WindowManager.h"
#include "Global.h"
#include "resource.h"
#include "ModuleRegister.h"
#include "EngineLoop.h"

LRESULT FWindowManager::WndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	std::unordered_map<HWND, FWindow*> & _hwndMap = Get().mHwndMap;
	auto it = _hwndMap.find(_hwnd);

	if (it == _hwndMap.end())
		return DefWindowProcW(_hwnd, _msg, _wParam, _lParam);
	return	it->second->WndProc(_hwnd, _msg, _wParam, _lParam);
}

void FWindowManager::Init(HINSTANCE _instance)
{
	mInstance = _instance;
	RegistClass(L"CmWindow", LoadIconW(mInstance, MAKEINTRESOURCEW(IDI_CMICON)));
}

bool FWindowManager::RegistClass(const wchar_t * _className, HICON _icon)
{
	WNDCLASSW wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.style = CS_DBLCLKS; // We want to receive double clicks
	wc.lpfnWndProc = &FWindowManager::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mInstance;
	wc.hIcon = _icon;
	wc.hCursor = NULL;	// We manage the cursor ourselves
	wc.hbrBackground = NULL; // Transparent
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _className;
	return !!RegisterClassW(&wc);
}

bool FWindowManager::UnregistClass(const wchar_t * _className)
{
	return !!UnregisterClassW(_className, mInstance);
}

void FWindowManager::AddHandleMap(HWND _hwnd, FWindow * _window)
{
	mHwndMap.insert(std::make_pair(_hwnd, _window));
}

void FWindowManager::EraseHandleMap(HWND _hwnd)
{
	mHwndMap.erase(_hwnd);
}

void FWindowManager::PumpMessages()
{
	MSG Message;

	// standard Windows message handling
	while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	if (FWindowManager::Get().GetWindowNumber() == 0)
		RequestExit(false);
}

RegistDefaultModule(FDefaultModuleImpl, SlateCore);
