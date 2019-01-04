#pragma once
#include <string>
#include <map>
#include <functional>
#include <vector>
#include <d3d12.h>
#include <d3dcommon.h>
#include "d3dx12.h"

#define Comment(...)

class FMaterialsManager
{
	FMaterialsManager() = default;
public:
	static FMaterialsManager & Get()
	{
		static FMaterialsManager single;
		return single;
	}

	void Init();
	void Save();
	void Clear();

	void AddMaterials(
		std::function<void(
			_Out_ Comment(VertexId) uint64_t &,
			_Out_ std::vector<CD3DX12_ROOT_PARAMETER> &,
			_Out_ std::vector<CD3DX12_STATIC_SAMPLER_DESC> &,
			_Out_ D3D12_ROOT_SIGNATURE_FLAGS &,
			_Out_ D3D_ROOT_SIGNATURE_VERSION &,
			_Out_ Comment(VS) ID3DBlob *&,
			_Out_ Comment(HS) ID3DBlob *&,
			_Out_ Comment(DS) ID3DBlob *&,
			_Out_ Comment(GS) ID3DBlob *&,
			_Out_ Comment(PS) ID3DBlob *&,
			_Out_ D3D12_GRAPHICS_PIPELINE_STATE_DESC &
			)
		> _materialsInitFunc,
		const std::wstring & _materialsName,
		const std::wstring & _destFileName
	);

private:
	std::map<std::wstring, std::wstring> mMap;
	std::wstring mManagerFileName = L"MaterialsManager.data";
};