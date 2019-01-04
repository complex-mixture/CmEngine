#pragma once
#include "ModuleManager.h"

template<typename _moduleClass, template<typename> typename ... _executor>
struct ModuleRegister
{
	using ModuleClass = _moduleClass;
	using MyType = ModuleRegister<_moduleClass, _executor...>;

	static_assert(std::is_base_of_v<IModuleInterface, ModuleClass>, "the module class is not drived from IModuleInterface");

	template<template<typename> typename _first, template<typename> typename ... _rest>
	__forceinline static void __Exec(const std::wstring & _moduleName)
	{
		_first<MyType>::Exec(_moduleName);
		__Exec<_rest...>(_moduleName);
	}

	template<typename t>
	__forceinline static void __Exec(const t&)
	{
	}

	ModuleRegister(const std::wstring & _moduleName)
	{
		__Exec<_executor...>(_moduleName);
	}
};

template<typename _moduleRegister>
struct MRE_Main
{
	using ModuleRegister = _moduleRegister;

	__forceinline static void Exec(const std::wstring & _moduleName)
	{
		FModuleInfo _moduleInfo;
		_moduleInfo.mModuleName = _moduleName;
		_moduleInfo.mModuleCreateFunction = []()->IModuleInterface* {return new ModuleRegister::ModuleClass(); };

		FModuleManager::Get().GetModuleInfos().insert(
			std::make_pair(
				_moduleName,
				_moduleInfo
			)
		);
	}
};

template<typename _moduleRegister>
struct MRE_Check
{
	__forceinline static void Exec(const std::wstring & _moduleName)
	{
		bool IsModuleRegisted = FModuleManager::Get().GetModuleInfos().count(_moduleName) > 0;
		if (IsModuleRegisted)
			DebugMessageBoxW(L"module error", L"%s module was registed for more than one times", _moduleName.c_str());
	}
};

template<typename _moduleRegister>
struct MRE_Log
{
	__forceinline static void Exec(const std::wstring & _moduleName)
	{
		LogW(L"the %s module is registed\n", _moduleName.c_str());
	}
};

#define RegistModule(_ModuleClass, _ModuleName, ...) ModuleRegister<_ModuleClass, __VA_ARGS__> __##_ModuleName##_moduleRegister(L###_ModuleName);

#ifdef DEBUG
#define RegistDefaultModule(_ModuleClass, _ModuleName) RegistModule(_ModuleClass, _ModuleName, MRE_Check, MRE_Main, MRE_Log)
#else
#define RegistDefaultModule(_ModuleClass, _ModuleName) RegistModule(_ModuleClass, _ModuleName, MRE_Main)
#endif // DEBUG
