#pragma once
enum class EEntityType
{
	Unknow = 0,
	Camera,
	DirectionLight,
	PontLight,
	SpotLight,
	SkyBox,
	SaticMesh,
	Num
};

#define SpecificEntityType(_entityType)\
	public:\
		static EEntityType GetStaticEntityType() { return _entityType; }\
		EEntityType GetEntityType()const override {return GetStaticEntityType(); }\
	private:
