#pragma once
#include <vector>
#include <tuple>
#include <unordered_map>

class AActor;

template<typename... _componentAndMaskTypes>
class FRenderWorldContext
{
	void Init(UWorld * _world);
	void Tick();
	void Clear();

	template<typename _componentType>
	auto& GetComponent();

	template<typename _maskType>
	auto& GetMask();

private:
	std::tuple<_componentAndMaskTypes...> mComponentAndMasks;
	std::unordered_map<AActor*, uint64_t> mActorMap;

};
