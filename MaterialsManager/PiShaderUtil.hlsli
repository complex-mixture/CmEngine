#ifndef __PI_SHADER_UTIL_HLSLI__
#define __PI_SHADER_UTIL_HLSLI__

#include "LightCommon.hlsli"

float3 ComputeLight_Pi(float3 _baseColor, float3 _normal, float _lightIntensityFactor, Light _light, float3 _toLight, float3 _toEye)
{
    float3 cosNL = dot(_normal, _toLight);
    float3 reflectLight = cosNL * 2.f * _normal;
    float factor = dot(reflectLight - _toLight, _toEye);
    factor = factor / 2.f + 0.5f;
    float factor2 = min(1.f - dot(_normal, _toEye), 1.f);
    factor2 = factor2 * factor2 * (3.f - 2.f * factor2);
    //factor *= min(1.f - dot(_normal, _toEye), 1.f);
    factor = factor * factor * (3.f - 2.f * factor);
    //factor *= factor2;
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
in const uint _directionLightIndexEnd,
in const uint _pointLightIndexEnd,
in const uint _relatedLightIndeices[255],
in const Light _lights[255],
in const float3 _normal,
in const float3 baseColor,
in const float3 toEye,
in const float3 _positionW
)
{
    float3 destColor;

    for (uint i = 0; i != _relatedLightCount; ++i)
    {
        if (_relatedLightIndeices[i] >= _directionLightIndexEnd)
        {
            destColor += ComputeDirectionLight_Pi(baseColor, _lights[_relatedLightIndeices[i]], toEye, _normal);
        }
        else if (_relatedLightIndeices[i] >= _pointLightIndexEnd)
        {
            destColor += ComputePointLight_Pi(baseColor, _lights[_relatedLightIndeices[i]], toEye, _normal, _positionW);
        }
        else
        {
            destColor += ComputeSpotLight_Pi(baseColor, _lights[_relatedLightIndeices[i]], toEye, _normal, _positionW);
        }
    }

    return destColor;
}


//float3 ComputeLights_Pi(
//in const uint _relatedLightCount,
//in const uint _relatedDirectionLightCount,
//in const uint _relatedPointLightCount,
//in const uint _relatedSpotLightCount,
//in const uint _directionLightIndexEnd,
//in const uint _pointLightIndexEnd,
//in const uint4 _relatedLightIndeices[64],
//in const Light _lights[256],
//in const float3 _normal,
//in const float3 baseColor,
//in const float3 toEye,
//in const float3 _positionW
//)
//{
//    float3 destColor;

//    //uint index = 0;
//    //uint i;
//    //for (i = 0; i != _relatedDirectionLightCount; ++index, ++index)
//    //{
//    //    destColor += ComputeDirectionLight_Pi(baseColor, _lights[_relatedLightIndeices[index / 4][index % 4]], toEye, _normal);
//    //}

//    //for (i = 0; i != _relatedPointLightCount; ++index, ++index)
//    //{
//    //    destColor += ComputePointLight_Pi(baseColor, _lights[_relatedLightIndeices[index / 4][index % 4]], toEye, _normal, _positionW);
//    //}

//    //for (i = 0; i != _relatedSpotLightCount; ++index, ++index)
//    //{
//    //    destColor += ComputeSpotLight_Pi(baseColor, _lights[_relatedLightIndeices[index / 4][index % 4]], toEye, _normal, _positionW);

//    //}

//    for (uint i = 0; i != _relatedLightCount; ++i)
//    {
//        if (_relatedLightIndeices[i] >= _directionLightIndexEnd)
//        {
//            destColor += ComputeDirectionLight_Pi(baseColor, _lights[_relatedLightIndeices[i]], toEye, _normal);
//        }
//        else if (_relatedLightIndeices[i] >= _pointLightIndexEnd)
//        {
//            destColor += ComputePointLight_Pi(baseColor, _lights[_relatedLightIndeices[i]], toEye, _normal, _positionW);
//        }
//        else
//        {
//            destColor += ComputeSpotLight_Pi(baseColor, _lights[_relatedLightIndeices[i]], toEye, _normal, _positionW);
//        }
//    }

//    return destColor;
//}

#endif//__PI_SHADER_UTIL_HLSLI__
