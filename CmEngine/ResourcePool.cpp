#include "ResourcePool.h"
#include "d3dx12.h"
#include "D3D12RhiGlobal.h"
#include "D3D12RhiUtil.h"

ID3D12Resource* FResourcePool::__AllocateResource(D3D12_RESOURCE_DESC _desc, const D3D12_CLEAR_VALUE* _clearValue)
{
	ID3D12Resource* newResource = nullptr;
	VerifyD3D12Result(GDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&_desc,
		D3D12_RESOURCE_STATE_COMMON,
		_clearValue,
		IID_PPV_ARGS(&newResource)
	));

	return newResource;
}

void FResourcePool::Tick()
{
	for (auto listIt = mMap.begin(); listIt != mMap.end(); NULL)
	{
		auto& list = listIt->second;
		for (auto resourceIt = list.begin(); resourceIt != list.end(); NULL)
		{
			if (resourceIt->mIsUsed == 0)
			{
				if (resourceIt->mLifeTime == 0)
				{
					resourceIt = list.erase(resourceIt);
					continue;
				}
				else
				{
					--resourceIt->mLifeTime;
				}
			}

			++resourceIt;
		}

		if (list.size() == 0)
			listIt = mMap.erase(listIt);
		else
			++listIt;
	}
}

void FResourcePool::Clear()
{
	for (auto listIt = mMap.begin(); listIt != mMap.end(); ++listIt)
	{
		auto& list = listIt->second;
		for (auto resourceIt = list.begin(); resourceIt != list.end(); ++resourceIt)
		{
			Assert(resourceIt->mIsUsed == 0);
			resourceIt->mResource->Release();
		}
	}
	mMap.clear();
}

ID3D12Resource* FResourcePool::AllocateResource(D3D12_RESOURCE_DESC _desc, const D3D12_CLEAR_VALUE* _clearValue)
{
	auto listIt = mMap.find(FResourceInfo(_desc, _clearValue));
	if (listIt == mMap.end())
	{
		listIt = mMap.insert(std::make_pair(FResourceInfo(_desc, _clearValue), std::list<FResource>())).first;
	}
	else
	{
		auto resourceIt = std::find_if(listIt->second.begin(), listIt->second.end(), [](const FResource & _resource) {return !_resource.mIsUsed; });
		if (resourceIt != listIt->second.end())
		{
			resourceIt->mIsUsed = true;
			return resourceIt->mResource;
		}
	}

	auto& list = listIt->second;

	FResource resource;
	resource.mResource = __AllocateResource(_desc, _clearValue);
	resource.mIsUsed = true;

	list.push_front(resource);

	return resource.mResource;
}

void FResourcePool::ReleaseResource(ID3D12Resource* _resource)
{
	for (auto listIt = mMap.begin(); listIt != mMap.end(); ++listIt)
	{
		auto& list = listIt->second;
		auto resourceIt = std::find_if(list.begin(), list.end(), [=](const FResource & _val) {return _val.mResource == _resource; });
		if (resourceIt != list.end())
		{
			Assert(resourceIt->mIsUsed == 1);
			resourceIt->mIsUsed = 0;
			resourceIt->mLifeTime = 0x7f;
			return;
		}
	}
}
