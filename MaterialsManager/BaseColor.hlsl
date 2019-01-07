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

SamplerState AnisotropicSampler : register(s0);
Texture2D BaseColor : register(t0);

struct VertexIn
{
    float3 position : POSITION;
    float2 uv : UV;
};

struct VertexOut
{
    float4 positionH : SV_POSITION;
    float2 uv : UV;
};

void VsMain(in VertexIn _in, out VertexOut _out)
{
    _out.positionH = mul(float4(_in.position, 1.0f), gWroldMatrix);
    _out.positionH = mul(_out.positionH, gViewProjMatrix);
    _out.uv = _in.uv;
}

float4 PsMain(in VertexOut _in) : SV_TARGET
{
    return BaseColor.Sample(AnisotropicSampler, _in.uv);
}
