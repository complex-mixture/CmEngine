#pragma once
#pragma once
#include <type_traits>
#include <map>
#include <string>
#include "Util.h"
#include "ModuleInterface.h"
#include "Template.h"


struct FModuleInfo
{
	using ModuleCreateFunction = IModuleInterface *(*)();

	std::wstring mModuleName = L"unnamedmodule";
	IModuleInterface * mModulePtr = nullptr;
	ModuleCreateFunction mModuleCreateFunction = nullptr;
};

class FModuleManager final : public Tpl::TAsSingle<FModuleManager>
{
public:
	IModuleInterface * LoadModule(std::wstring _moduleName)
	{
		auto it = GetModuleInfos().find(_moduleName);

		if (it == GetModuleInfos().end())
			DebugMessageBoxW(L"module error", L"%s module was not registed", _moduleName.c_str());

		FModuleInfo & _moduleInfo = it->second;

		if (_moduleInfo.mModulePtr != nullptr)
			DebugMessageBoxW(L"module error", L"%s module was loaded", _moduleName.c_str());

		_moduleInfo.mModulePtr = _moduleInfo.mModuleCreateFunction();
		_moduleInfo.mModulePtr->Init();

		return _moduleInfo.mModulePtr;
	}

	template<class _moduleClass = IModuleInterface>
	_moduleClass * GetModule(std::wstring _moduleName)
	{
		static_assert(std::is_base_of_v<IModuleInterface, _moduleClass>, "the module class is not drived from IModuleInterface");
		auto it = GetModuleInfos().find(_moduleName);

		if (it == GetModuleInfos().end())
			DebugMessageBoxW(L"module error", L"%s module was not registed", _moduleName.c_str());

		FModuleInfo & _moduleInfo = it->second;
		
		if (_moduleInfo.mModulePtr == nullptr)
			DebugMessageBoxW(L"module error", L"%s module was not loaded", _moduleName.c_str());


		return dynamic_cast<_moduleClass*>(_moduleInfo.mModulePtr);
	}

	bool UnloadModule(std::wstring _moduleName)
	{
		auto it = GetModuleInfos().find(_moduleName);
		if (it == GetModuleInfos().end())
			DebugMessageBoxW(L"module error", L"%s module was not registed", _moduleName.c_str());

		FModuleInfo & _moduleInfo = it->second;

		Assert(_moduleInfo.mModulePtr != nullptr);

		_moduleInfo.mModulePtr->Clear();
		delete _moduleInfo.mModulePtr;
		_moduleInfo.mModulePtr = nullptr;
		return true;
	}

	std::map<std::wstring, FModuleInfo> & GetModuleInfos()
	{
		static std::map<std::wstring, FModuleInfo> _modules;
		return _modules;
	}

	void Exit()
	{
		uint32_t Count = 0;
		for (const auto & _ele : GetModuleInfos())
		{
			if (_ele.second.mModulePtr != nullptr)
			{
				DebugMessageBox(TEXT("module error"), TEXT("%s module was not unload"), _ele.second.mModuleName.c_str());
				Count++;
			}
		}
		Assert(Count == 0 && "some module was not unload");
	}

};
