#pragma once
#include <stdio.h>
#include <windows.h>
#include "Common.h"
#include <assert.h>

#define LogA(_format, ...)					\
do											\
{											\
	char buffer[1024];						\
	sprintf_s(buffer, _format, __VA_ARGS__);\
	OutputDebugStringA(buffer);				\
} while (0)

#define LogW(_format, ...)						\
do												\
{												\
	char buffer[1024];							\
	swprintf_s(buffer, _format, __VA_ARGS__);	\
	OutputDebugStringW(buffer);					\
} while (0)

#ifdef UNICODE
#define Log LogA
#else
#define Log LogW
#endif

#ifdef DEBUG
#define DebugMessageBoxA(_caption, _format, ...)									\
do																					\
{																					\
	char buffer[1024];																\
	sprintf_s(buffer, _format, __VA_ARGS__);										\
	MessageBoxA(0, buffer, _caption, 0);											\
	__debugbreak();																	\
} while (0)

#define DebugMessageBoxW(_caption, _format, ...)									\
do																					\
{																					\
	wchar_t buffer[1024];															\
	swprintf_s(buffer, _format, __VA_ARGS__);										\
	MessageBoxW(0, buffer, _caption, 0);											\
	__debugbreak();																	\
} while (0)

#else
#define  DebugMessageBoxA(...) ((void)(0))
#define  DebugMessageBoxW(...) ((void)(0))
#endif // DEBUG

#ifdef UNICODE
#define DebugMessageBox DebugMessageBoxW
#else
#define DebugMessageBox DebugMessageBoxA
#endif

#ifdef DEBUG
#define AssertDebugMessageBoxW(_expression, _caption, _format, ...) \
do {																\
	if (!!(_expression))											\
		DebugMessageBoxW(_caption, _format, __VA_ARGS__);			\
} while (0)

#define AssertDebugMessageBoxA(_expression, _caption, _format, ...) \
do {																\
	if (!!(_expression))											\
		DebugMessageBoxA(_caption, _format, __VA_ARGS__);			\
} while (0)
#else
#define AssertDebugMessageBoxW ((void)0)
#define AssertDebugMessageBoxA ((void)0)
#endif

#ifdef UNICODE
#define AssertDebugMessageBox AssertDebugMessageBoxW
#else
#define AssertDebugMessageBox AssertDebugMessageBoxA
#endif

#ifdef DEBUG
#define Assert(_expression)															\
do																					\
{																					\
	if (!(_expression))																\
		DebugMessageBox(															\
			TEXT("assert failed"),													\
			TEXT("file: %s\n" "func: %s\n" "line: %d\n" "%s"),						\
			TEXT(__FILE__), TEXT(__FUNCTION__),  (int)__LINE__, L###_expression);	\
} while (0)
#else
#define Assert ((void)0)
#endif

/**
* utility template for a class that should not be copyable.
* Derive from this class to make your class non-copyable
*/
class FNoncopyable
{
protected:
	// ensure the class cannot be constructed directly
	FNoncopyable() {}
	// the class should not be used polymorphically
	~FNoncopyable() {}
private:
	FNoncopyable(const FNoncopyable&) = delete;
	FNoncopyable& operator=(const FNoncopyable&) = delete;
};