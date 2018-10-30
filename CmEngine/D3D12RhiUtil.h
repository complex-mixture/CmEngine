#pragma once
#include "Util.h"
#include <comdef.h>

#define VerifyD3D12Result(_x)																	\
do																								\
{																								\
	HRESULT hr = (_x);																			\
	if(hr != S_OK)																				\
		DebugMessageBox(TEXT("D3D12 HR FAILED"),												\
			TEXT("file: %s\n" "func: %s\n" "line: %d\n" "error: %s\n"),							\
			TEXT(__FILE__), TEXT(__FUNCTION__), (int)__LINE__, _com_error(hr).ErrorMessage());	\
} while (0)
