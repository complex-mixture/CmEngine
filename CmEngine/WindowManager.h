#pragma once
#include <unordered_map>
#include <windef.h>
#include "Util.h"
#include "Template.h"
class FWindow;

class FWindowManager final : public Tpl::TAsSingle<FWindowManager>
{
private:
	HINSTANCE mInstance = NULL;
	std::unordered_map<HWND, FWindow*> mHwndMap;

public:
	void Init(HINSTANCE _instance);
	bool RegistClass(const wchar_t * _className, HICON _icon);
	bool UnregistClass(const wchar_t * _className);
	void AddHandleMap(HWND _hwnd, FWindow* _window);
	void EraseHandleMap(HWND _hwnd);
	void PumpMessages();
	__forceinline size_t GetWindowNumber()const { return mHwndMap.size(); }

	__forceinline HINSTANCE GetInstance()const { return mInstance; }

private:
	static LRESULT CALLBACK WndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);
};
