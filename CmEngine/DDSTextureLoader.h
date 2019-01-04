//--------------------------------------------------------------------------------------
// File: DDSTextureLoader.h
//
// Functions for loading a DDS texture and creating a Direct3D 11 runtime resource for it
//
// Note these functions are useful as a light-weight runtime loader for DDS files. For
// a full-featured DDS file reader, writer, and texture processing pipeline see
// the 'Texconv' sample and the 'DirectXTex' library.
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkId=248926
// http://go.microsoft.com/fwlink/?LinkId=248929
//--------------------------------------------------------------------------------------

#ifdef _MSC_VER
#pragma once
#endif

#include <wrl.h>
#include <d3d11_1.h>
#include "d3dx12.h"

#pragma warning(push)
#pragma warning(disable : 4005)
#include <stdint.h>
#include <memory>

#pragma warning(pop)

#if defined(_MSC_VER) && (_MSC_VER<1610) && !defined(_In_reads_)
#define _In_reads_(exp)
#define _Out_writes_(exp)
#define _In_reads_bytes_(exp)
#define _In_reads_opt_(exp)
#define _Outptr_opt_
#endif

#ifndef _Use_decl_annotations_
#define _Use_decl_annotations_
#endif

namespace DirectX
{
    enum DDS_ALPHA_MODE
    {
        DDS_ALPHA_MODE_UNKNOWN       = 0,
        DDS_ALPHA_MODE_STRAIGHT      = 1,
        DDS_ALPHA_MODE_PREMULTIPLIED = 2,
        DDS_ALPHA_MODE_OPAQUE        = 3,
        DDS_ALPHA_MODE_CUSTOM        = 4,
    };
	HRESULT CreateDDSTextureFromFile12(_In_ ID3D12Device* device,
		                               _In_ ID3D12GraphicsCommandList* cmdList,
		                               _In_z_ const wchar_t* szFileName,
		                               _Out_ Microsoft::WRL::ComPtr<ID3D12Resource>& texture,
		                               _Out_ Microsoft::WRL::ComPtr<ID3D12Resource>& textureUploadHeap,
									   _Out_ std::unique_ptr<uint8_t[]>& pData,
									   _Out_ std::unique_ptr<D3D12_SUBRESOURCE_DATA[]>& pSubResourceData,
									   _Out_ UINT * pNumSubresource,
		                               _In_ size_t maxsize = 0,
		                               _Out_opt_ DDS_ALPHA_MODE* alphaMode = nullptr
		                               );
}
