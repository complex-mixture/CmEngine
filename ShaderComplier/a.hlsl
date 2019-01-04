
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
Texture2D NormalMap : register(t1);

struct VertexIn
{
    	//struct Vertex_default
	//{
	//	static constexpr uint64_t VertexId = 1;

	//	DirectX::XMFLOAT3 mPosition;
	//	DirectX::XMFLOAT2 mUV;
	//	DirectX::XMFLOAT3 mTangent;
	//	DirectX::XMFLOAT3 mBinomal;
	//};

    float3 position : POSITION;
    float2 uv : UV;
    //float3 normal : NORMAL;
    float3 targent : TANGENT;
    float3 binormal : BINORMAL;
};

struct VertexOut
{
    float4 position : SV_POSITION;
    float3 worldPosition : WORLD_POSITION;
    float2 uv : UV;
    //float3 normalW : NORMAL;
    float3 targentW : TANGENT;
    float3 binormalW : BINORMAL;
};

VertexOut VSMain(VertexIn _in)
{
    VertexOut vout;
    vout.worldPosition = mul(float4(_in.position, 1.0f), gWroldMatrix).xyz;
    vout.position = mul(float4(vout.worldPosition, 1.0f), gViewProjMatrix);
    vout.targentW = mul(_in.targent, (float3x3) gWroldMatrix).xyz;
    vout.binormalW = mul(_in.binormal, (float3x3) gWroldMatrix).xyz;
    //vout.normalW = mul(float4(_in.normal, 1.0f), gWroldMatrix).xyz;
    vout.uv = _in.uv;
    return vout;
};

float4 PSMain(VertexOut _in) : SV_TARGET
{
    float4 basecolor = BaseColor.Sample(AnisotropicSampler, _in.uv);
    
    float3 normal = NormalMap.Sample(AnisotropicSampler, _in.uv).xyz;
    normal = normal * 2.f - 1.f;
    _in.targentW = normalize(_in.targentW);
    _in.binormalW = normalize(_in.binormalW);
    float3 normalW = cross(_in.binormalW, _in.targentW);
    normal = mul(normal, float3x3(_in.binormalW, _in.targentW, normalW));
    normal = normalize(normal);
    float3 destColor = basecolor.rgb * gAmbientLight;
    float3 toEye = normalize(gEyePosition - _in.worldPosition);

    float3 coolColor = float3(0.3f, 0.3f, 0.f) + 0.25f * basecolor.rgb;
    float3 warmColor = float3(0.f, 0.f, 0.55f) + 0.25f * basecolor.rgb;

    Light a;
    a.mIntensity = 1.f;
    a.mColor = gAmbientLight;
    a.mDirection = -normalW;

    destColor += ComputeDirectionalLight(a, normal, toEye, basecolor.rbg);


    for (uint i = 0; i != gRelatedLightCount; ++i)
    {
        if (gRelatedLightIndeices[i] < gPointLightIndexStart)
        {
            float3 toLight;
            float intensity;
            destColor += ComputeDirectionalLight(gLights[gRelatedLightIndeices[i]], normal, toEye, basecolor.rbg);
            //ComputeToEyeAndIntensity_DirectionLight(gLights[gRelatedLightIndeices[i]], toLight, intensity);
            //destColor += ComputeLight(coolColor, warmColor, normal, toLight, toEye);
        }
        //else if (gRelatedLightIndeices[i] < gSpotLightIndexStart)
        //{
        //    destColor += ComputePointLight(gLights[gRelatedLightIndeices[i]], basecolor, _in.worldPosition, _in.normal, toEye);
        //}
        //else
        //{
        //    destColor += ComputeSpotLight(gLights[gRelatedLightIndeices[i]], basecolor, _in.worldPosition, _in.normal, toEye);
        //}
    }
    normal = (normal + 1.f) / 2.f;
    return float4(normal, 1.0f);
    return float4(destColor, 1.0f);
}
