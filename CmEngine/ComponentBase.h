#pragma once

enum class ComponentType
{
	BaseComponent,
	PreTreatedComponent,
	TreatedComponent
};

template<typename... _depdences>
struct ComponentBase
{
	using Depdences = Tpl::TypeSequence<_depdences...>;

	template<typename _renderWorldContextType>
	void Update(const _renderWorldContextType * _renderWorldContext) = delete;

	inline void Resize(uint64_t _newSize)
	{
		mComponents.resize(_newSize);
		mDirtyFlags.resize(_newSize, true);
	}

	__forceinline size_t GetSize()const { return mComponents.size(); }
	__forceinline const std::vector<ComponentType> & GetComponents()const { return mComponents; }

private:
	std::vector<ComponentType> mComponents;
	std::vector<bool> mDirtyFlags;
};
