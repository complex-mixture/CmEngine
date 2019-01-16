#ifndef __PI_SHADER_UTIL_HLSLI__
#define __PI_SHADER_UTIL_HLSLI__

#include "LightCommon.hlsli"

float3 ComputeLight_Pi(float3 _baseColor, float3 _normal, float _lightIntensityFactor, Light _light, float3 _toLight, float3 _toEye)
{
    float3 cosNL = dot(_normal, _toLight);
    float3 reflectLight = cosNL * 2.f * _normal;
    float factor = dot(reflectLight - _toLight, _toEye);
    factor = factor / 2.f + 0.5f;
    factor = factor * factor * (3.f - 2.f * factor);
    return _light.mIntensity * _lightIntensityFactor * factor * _light.mColor * _baseColor;
}

float3 ComputeDirectionLight_Pi(float3 _baseColor, Light _directionLight, float3 _toEye, float3 _normal)
{
    return ComputeLight_Pi(_baseColor, _normal, 1.f, _directionLight, -_directionLight.mDirection, _toEye);
}

float3 ComputePointLight_Pi(float3 _baseColor, Light _pointLight, float3 _toEye, float3 _normal, float3 _worldPosition)
{
    float3 toLight = _pointLight.mPosition - _worldPosition;
    float dis = distance(_pointLight.mPosition, _worldPosition);
    toLight = normalize(toLight);
    float factor = ComputeDistanceFalloffFactor_PunctualLight(dis, _pointLight.mFallOffBeginSqr, _pointLight.mFalloffEnd);
    return ComputeLight_Pi(_baseColor, _normal, factor, _pointLight, toLight, _toEye);
}

float3 ComputeSpotLight_Pi(float3 _baseColor, Light _spotLight, float3 _toEye, float3 _normal, float3 _worldPosition)
{
    float3 toLight = _spotLight.mPosition - _worldPosition;
    float dis = distance(_spotLight.mPosition, _worldPosition);
    toLight = normalize(toLight);
    float factor = ComputeDistanceFalloffFactor_PunctualLight(dis, _spotLight.mFallOffBeginSqr, _spotLight.mFalloffEnd);
    factor *= ComputeDirectionFallOffFactor_SpotLight(_spotLight.mDirection, toLight, _spotLight.mCosHalfInnerConeAngle, _spotLight.mCosHalfOuterConeAngle);
    return ComputeLight_Pi(_baseColor, _normal, factor, _spotLight, toLight, _toEye);
}

float3 ComputeLights_Pi(
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
            destColor += ComputeSpotLight_Pi(baseColor, _lights[_relatedLightIndeices[i / 4][i % 4]], toEye, _normal, _positionW);
        }
        else if (_relatedLightIndeices[i / 4][i % 4] >= _pointLightIndexStart)
        {
            destColor += ComputePointLight_Pi(baseColor, _lights[_relatedLightIndeices[i / 4][i % 4]], toEye, _normal, _positionW);
        }
        else
        {
            destColor += ComputeDirectionLight_Pi(baseColor, _lights[_relatedLightIndeices[i / 4][i % 4]], toEye, _normal);
        }
    }

    return destColor;
}

#endif//__PI_SHADER_UTIL_HLSLI__
