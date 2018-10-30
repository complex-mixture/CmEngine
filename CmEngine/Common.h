#pragma once
#include <stdint.h>

#if defined(DEBUG) || defined(_DEBUG)
#define DEBUG
constexpr bool IsDebugMode = true;
constexpr bool IsReleaseMode = false;
#else
constexpr bool IsDebugMode = false;
constexpr bool IsReleaseMode = true;
#endif
