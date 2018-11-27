#pragma once
__interface IModuleInterface
{
	void Init();
	void Clear();
};

struct FDefaultModuleImpl : IModuleInterface
{
	void Init() {}
	void Clear() {}
};