#ifndef __GOOCH_SHADER_UTIL_HLSLI__
#define __GOOCH_SHADER_UTIL_HLSLI__

#include "LightCommon.hlsli"

void ComputeWarmAndCoolLight_Gooch(in float3 _baseColor, out float3 _warmColor, out float3 _coolColor)
{
    _warmColor = float3(0.3f, 0.3f, 0.f) + 0.25f * _baseColor;
    _coolColor = float3(0.f, 0.f, 0.55f) + 0.25f * _baseColor;
}

float3 ComputeLight_Gooch(float3 _baseColor, float3 _normal, float _lightIntensityFactor, Light _light, float3 _toLight, float3 _toEye)
{
    float3 warmColor, coolColor;
    ComputeWarmAndCoolLight_Gooch(_baseColor * _lightIntensityFactor * _light.mIntensity * _light.mColor, warmColor, coolColor);
    float t = (dot(_normal, _toLight) + 1.f) / 2.f;
    float3 r = 2 * dot(_normal, _toLight) * _normal - 1.f;
    float s = clamp((100.f * dot(r, _toEye) - 97.f) / 3.f, 0.f, 1.f);
    return lerp(warmColor, coolColor, t);
    //return lerp(float3(1.f, 1.f, 1.f), lerp(warmColor, coolColor, t), s);
}

float3 ComputeDirectionLight_Gooch(float3 _baseColor, Light _directionLight, float3 _toEye, float3 _normal)
{
    return ComputeLight_Gooch(_baseColor, _normal, 1.f, _directionLight, -_directionLight.mDirection, _toEye);
}

float3 ComputePointLight_Gooch(float3 _baseColor, Light _pointLight, float3 _toEye, float3 _normal, float3 _worldPosition)
{
    float3 toLight = _pointLight.mPosition - _worldPosition;
    float dis = distance(_pointLight.mPosition, _worldPosition);
    toLight = normalize(toLight);
    float factor = ComputeDistanceFalloffFactor_PunctualLight(dis, _pointLight.mFallOffBeginSqr, _pointLight.mFalloffEnd);
    return ComputeLight_Gooch(_baseColor, _normal, factor, _pointLight, toLight, _toEye);
}

float3 ComputeSpotLight_Gooch(float3 _baseColor, Light _spotLight, float3 _toEye, float3 _normal, float3 _worldPosition)
{
    float3 toLight = _spotLight.mPosition - _worldPosition;
    float dis = distance(_spotLight.mPosition, _worldPosition);
    toLight = normalize(toLight);
    float factor = ComputeDistanceFalloffFactor_PunctualLight(dis, _spotLight.mFallOffBeginSqr, _spotLight.mFalloffEnd);
    factor *= ComputeDirectionFallOffFactor_SpotLight(_spotLight.mDirection, toLight, _spotLight.mCosHalfInnerConeAngle, _spotLight.mCosHalfOuterConeAngle);
    return ComputeLight_Gooch(_baseColor, _normal, factor, _spotLight, toLight, _toEye);
}

float3 ComputeLights_Gooch(
in const uint _relatedLightCount,
in const uint _pointLightIndexStart,
in const uint _spotLightIndexStart,
in const uint4 _relatedLightIndeices[64],
in const Light _lights[256],
in const float3 _normal,
in const float3 baseColor,
in const float3 toEye,
in const float3 _positionW
)
{
    float3 destColor = 0.f;

    for (uint i = 0; i != _relatedLightCount; ++i)
    {
        if (_relatedLightIndeices[i / 4][i % 4] >= _spotLightIndexStart)
        {
            destColor += ComputeSpotLight_Gooch(baseColor, _lights[_relatedLightIndeices[i / 4][i % 4]], toEye, _normal, _positionW);
        }
        else if (_relatedLightIndeices[i / 4][i % 4] >= _pointLightIndexStart)
        {
            destColor += ComputePointLight_Gooch(baseColor, _lights[_relatedLightIndeices[i / 4][i % 4]], toEye, _normal, _positionW);
        }
        else
        {
            destColor += ComputeDirectionLight_Gooch(baseColor, _lights[_relatedLightIndeices[i / 4][i % 4]], toEye, _normal);
        }
    }

    return destColor;
}

#endif//__GOOCH_SHADER_UTIL_HLSLI__
