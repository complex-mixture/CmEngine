struct Light
{
    float3 mPosition;
    float mIntensity;
    float3 mColor;
    float mFalloffEnd;
    float3 mDirection;
    float mFalloffStart;
    float mInnerConeAngle;
    float mOuterConeAngle;
    float2 unsued[2];
};

float3 ComputeLight(float3 _coolColor, float3 _warmColor, float3 _normal, float3 _toLight, float3 _toEye)
{
    float t = (dot(_normal, _toLight) + 1.f) / 2.f;
    float3 r = 2 * dot(_normal, _toLight) * _normal - 1.f;
    float s = clamp(100.f * dot(r, _toEye) - 97.f, 0.f, 1.f);

    return /*s * float3(1.f, 1.f, 1.f) + (1 - s) **/ lerp(_warmColor, _coolColor, t);
}

void ComputeToEyeAndIntensity_DirectionLight(in Light _light, out float3 _toLight, out float _intensity)
{
    _intensity = _light.mIntensity;
    _toLight = -_light.mDirection;
}

float3 SchlickFresnel(float3 R0, float3 normal, float3 lightVec)
{
    float cosIncidentAngle = saturate(dot(normal, lightVec));

    float f0 = 1.0f - cosIncidentAngle;
    float3 reflectPercent = R0 + (1.0f - R0) * (f0 * f0 * f0 * f0 * f0);

    return reflectPercent;
}

float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal, float3 toEye, float3 baseColor)
{
    const float m = 0.125f * 256.0f;
    float3 halfVec = normalize(toEye + lightVec);

    float roughnessFactor = (m + 8.0f) * pow(max(dot(halfVec, normal), 0.0f), m) / 8.0f;
    float3 fresnelFactor = SchlickFresnel(float3(0.01f, 0.01f, 0.01f), halfVec, lightVec);

    float3 specAlbedo = fresnelFactor * roughnessFactor;

    // Our spec formula goes outside [0,1] range, but we are 
    // doing LDR rendering.  So scale it down a bit.
    specAlbedo = specAlbedo / (specAlbedo + 1.0f);

    return (baseColor + specAlbedo) * lightStrength;
}

//---------------------------------------------------------------------------------------
// Evaluates the lighting equation for directional lights.
//---------------------------------------------------------------------------------------
float3 ComputeDirectionalLight(Light L, float3 normal, float3 toEye, float3 baseColor)
{
    // The light vector aims opposite the direction the light rays travel.
    float3 lightVec = -L.mDirection;

    // Scale light down by Lambert's cosine law.
    float ndotl = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = L.mIntensity * ndotl;

    return BlinnPhong(lightStrength, lightVec, normal, toEye, baseColor);
}
