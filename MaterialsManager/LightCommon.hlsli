#ifndef __LIGHT_COMMON_HLSLI__
#define __LIGHT_COMMON_HLSLI__

struct Light
{
    float3 mPosition;
    float mIntensity;
    float3 mColor;
    float mFallOffBegin;
    float3 mDirection;
    float mFalloffEnd;
    float mCosHalfInnerConeAngle;
    float mCosHalfOuterConeAngle;
    float mFallOffBeginSqr;
    float mFallOffEndSqr;
};

float ComputeDistanceFalloffFactor_PunctualLight(in float _distance, in float _fallOffBeginSqr, in float _fallOffEnd)
{
    float factor = min(_fallOffBeginSqr / (_distance * _distance + 0.01f), 1.f);
    float t = _distance / _fallOffEnd;
    float winFactor = max(1 - t * t * t * t * t, 0);
    winFactor *= winFactor;
    return factor * winFactor;
}

float ComputeDirectionFallOffFactor_SpotLight(in float3 _lightDirection, float3 _toLight, float _cosHalfInnerConeAngle, float _cosHalfOuterConeAngle)
{
    float t = clamp((dot(_lightDirection, -_toLight) - _cosHalfOuterConeAngle) / (_cosHalfInnerConeAngle - _cosHalfOuterConeAngle), 0.f, 1.f);
    return t * t * (3 - 2 * t);
}

#endif//__LIGHT_COMMON_HLSLI__
