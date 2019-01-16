#ifndef __UTIL_HLSLI__
#define __UTIL_HLSLI__

// the _worldBinormal and _worldTangent should be normalized
float3x3 ComputeTbnMatrix(float3 _worldBinormal, float3 _worldTangent)
{
    return float3x3(_worldBinormal, _worldTangent, cross(_worldBinormal, _worldTangent));
}

float3 SampleNormalMap(float3x3 _tbnMatrix, Texture2D _texture, SamplerState _sampler, float2 _uv)
{
    float3 normal = _texture.Sample(_sampler, _uv).xyz;
    normal = normal * 2.f - 1.f;
    return mul(normal, _tbnMatrix);
}

float4 SampleTexture1d(Texture1D _texture, SamplerState _sampler, float _u)
{
    return _texture.Sample(_sampler, _u);
}

float4 SampleTexture2d(Texture2D _texture, SamplerState _sampler, float2 _uv)
{
    return _texture.Sample(_sampler, _uv);
}

float4 SampleTexture3d(Texture3D _texture, SamplerState _sampler, float3 _uvw)
{
    return _texture.Sample(_sampler, _uvw);
}

float4 SampleTextureCube(TextureCube _texture, SamplerState _sampler, float3 _xyz)
{
    return _texture.Sample(_sampler, _xyz);
}

#endif//__UTIL_HLSLI__
