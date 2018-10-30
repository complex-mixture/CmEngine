#pragma once
#pragma once
#include <type_traits>
#include <map>
#include <string>
#include "Util.h"
#include "ModuleInterface.h"


struct FModuleInfo
{
	using ModuleCreateFunction = IModuleInterface *(*)();

	std::string mModuleName = "unnamedmodule";
	IModuleInterface * mModulePtr = nullptr;
	ModuleCreateFunction mModuleCreateFunction = nullptr;
};

class FModuleManager
{
public:
	template<typename _moduleClass = IModuleInterface>
	_moduleClass * LoadModule(std::string _moduleName)
	{
		static_assert(std::is_base_of_v<IModuleInterface, _moduleClass>, "the module class is not drived from IModuleInterface");
		auto it = GetModuleInfos().find(_moduleName);

		if (it == GetModuleInfos().end())
			DebugMessageBoxA("module error", "%s module was not registed", _moduleName.c_str());

		FModuleInfo & _moduleInfo = it->second;
		if (_moduleInfo.mModulePtr == nullptr)
		{
			_moduleInfo.mModulePtr = _moduleInfo.mModuleCreateFunction();
			_moduleInfo.mModulePtr->Init();
		}

		return _moduleInfo.mModulePtr;
	}

	bool UnloadModule(std::string _moduleName)
	{
		auto it = GetModuleInfos().find(_moduleName);
		if (it == GetModuleInfos().end())
			DebugMessageBoxA("module error", "%s module was not registed", _moduleName.c_str());

		FModuleInfo & _moduleInfo = it->second;

		if (_moduleInfo.mModulePtr == nullptr)
			return false;

		_moduleInfo.mModulePtr->Clear();
		delete _moduleInfo.mModulePtr;
		_moduleInfo.mModulePtr = nullptr;
		return true;
	}

	static FModuleManager & Get()
	{
		static FModuleManager _single;
		return _single;
	}

	std::map<std::string, FModuleInfo> & GetModuleInfos()
	{
		static std::map<std::string, FModuleInfo> _modules;
		return _modules;
	}
};
