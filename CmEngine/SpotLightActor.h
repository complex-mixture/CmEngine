#pragma once
#include "PointLightActor.h"
class ASpotLightActor : public APointLightActor
{
	SpecificEntityType(EEntityType::SpotLight)

public:
	__forceinline void SetInnerConeAngle(float _innerConeAngle) { mInnerConeAngle = _innerConeAngle; }
	__forceinline void SetOuterConeAngle(float _outerConeAngle) { mOuterConeAngle = _outerConeAngle; }

	__forceinline float GetInnerConeAngle()const { return mInnerConeAngle; }
	__forceinline float GetOuterConeAngle()const { return mOuterConeAngle; }

private:
	float mInnerConeAngle;
	float mOuterConeAngle;
};
