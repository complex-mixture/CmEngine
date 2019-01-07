#include "Util.hlsli"
#include "LightCommon.hlsli"
#include "GoochShaderUtil.hlsli"

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
Texture2D NormalMap : register(t1);

struct VertexIn
{
    float3 position : POSITION;
    float2 uv : UV;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

struct VertexOut
{
    float4 positionH : SV_POSITION;
    float3 positionW : POSITIONW;
    float2 uv : UV;
    float3 tangentW : TANGENT;
    float3 binormalW : BINORMAL;
};

void VsMain(in VertexIn _in, out VertexOut _out)
{
    _out.positionW = mul(float4(_in.position, 1.f), gWroldMatrix).xyz;
    _out.positionH = mul(float4(_out.positionW, 1.f), gViewProjMatrix);
    _out.uv = _in.uv;
    _out.tangentW = mul(_in.tangent, (float3x3) gWroldMatrix);
    _out.binormalW = mul(_in.binormal, (float3x3) gWroldMatrix);
}

float4 PsMain(in VertexOut _in) : SV_Target
{
    _in.binormalW = normalize(_in.binormalW);
    _in.tangentW = normalize(_in.tangentW);

    float3x3 tbn = ComputeTbnMatrix(_in.binormalW, _in.tangentW);

    float3 normal = SampleNormalMap(tbn, NormalMap, AnisotropicSampler, _in.uv).xyz;
    float3 baseColor = SampleTexture2d(BaseColor, AnisotropicSampler, _in.uv).rbg;
    float3 toEye = gEyePosition - _in.positionW;

    float3 destColor = baseColor * gAmbientLight;

    for (uint i = 0; i != gRelatedLightCount; ++i)
    {
        if (gRelatedLightIndeices[i] < gPointLightIndexStart)
        {
            destColor += ComputeDirectionLight_Gooch(baseColor, gLights[gRelatedLightIndeices[i]], toEye, normal);
        }
        else if (gRelatedLightIndeices[i] < gSpotLightIndexStart)
        {
            destColor += ComputePointLight_Gooch(baseColor, gLights[gRelatedLightIndeices[i]], toEye, normal, _in.positionW);
        }
        else
        {
            destColor += ComputeSpotLight_Gooch(baseColor, gLights[gRelatedLightIndeices[i]], toEye, normal, _in.positionW);
        }
    }

    return float4(destColor, 1.f);
}