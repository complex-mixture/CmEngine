#pragma once
#include <d3d12.h>
#include <stdint.h>
#include <map>
#include <list>
#include "Util.h"

class FResourcePool
{
private:
	ID3D12Resource* __AllocateResource(D3D12_RESOURCE_DESC _desc, const D3D12_CLEAR_VALUE* _clearValue);

public:
	static FResourcePool& Get()
	{
		static FResourcePool single;
		return single;
	}

	void Init(){}
	void Tick();
	void Clear();

	ID3D12Resource * AllocateResource(D3D12_RESOURCE_DESC _desc, const D3D12_CLEAR_VALUE * _clearValue);
	void ReleaseResource(ID3D12Resource * _texture);

private:
	struct FResourceInfo
	{
		D3D12_RESOURCE_DESC mDesc;
		bool mWithClearValue;
		D3D12_CLEAR_VALUE mClearValue;

		FResourceInfo(const D3D12_RESOURCE_DESC & _desc, const D3D12_CLEAR_VALUE * _clearValue)
			:mDesc(_desc), mWithClearValue(!!_clearValue)
		{
			if (mWithClearValue)
			{
				mClearValue = *_clearValue;
			}
			else
			{
				memset(&mClearValue, 0x00, sizeof(D3D12_CLEAR_VALUE));
			}
		}
	};

	struct FResource
	{
		ID3D12Resource* mResource;
		uint8_t mIsUsed : 1;
		uint8_t mLifeTime : 7;
	};

	std::map<FResourceInfo, std::list<FResource>, FLess<FResourceInfo>> mMap;
};
