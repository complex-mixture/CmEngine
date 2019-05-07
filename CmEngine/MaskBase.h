#pragma once

template<typename... _depdences>
struct FMaskBase
{
	using Depdences = Tpl::TypeSequence<_depdences...>;

	template<typename _renderWorldContextType>
	void Update(const _renderWorldContextType * _renderWorldContext) = delete;

	void Resize(uint64_t _newSize)
	{
		mMasks.resize(_newSize, true);
	}

protected:
	std::vector<bool> mMasks;
};