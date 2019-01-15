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

constexpr float PI = 3.1415928f;
constexpr float PI2 = 2 * PI;

#pragma warning(disable : 4244 4267)
