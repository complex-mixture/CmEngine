#include "LightCommon.hlsli"

cbuffer PassCb : register(b0)
{
    float4x4 gViewMatrix;
    float4x4 gVireMatrixInv;
    float4x4 gProjMatrix;
    float4x4 gProjMatrixInv;
    float4x4 gViewProjMatrix;
    float4x4 gViewProjMatrixInv;
    float2 gRenderTargetSize;
    float2 gRenderTargetSizeInv;
    float3 gEyePosition;
    float gDeltaTime;
    float3 gAmbientLight;
    float gTotalTime;
    uint gPointLightIndexStart;
    uint gSpotLightIndexStart;
    float2 unused;
    Light gLights[255];
}

cbuffer ObjCb : register(b1)
{
    float4x4 gWroldMatrix;
    float4x4 gWroldMatrixInv;
    uint gRelatedLightCount;
    uint gRelatedLightIndeices[255];
}

SamplerState LinerSampler : register(s0);
TextureCube BaseColor : register(t0);

struct VertexIn
{
    float3 position : POSITION;
};

struct VertexOut
{
    float4 positionH : SV_POSITION;
    float3 position : POSITION;
};

void VsMain(in VertexIn _in, out VertexOut _out)
{
    _out.positionH = mul(float4(_in.position + gEyePosition, 1.f), gViewProjMatrix);
    _out.position = _in.position;
}

float4 PsMain(in VertexOut _in) : SV_TARGET
{
    return BaseColor.Sample(LinerSampler, _in.position.xyz);
}
