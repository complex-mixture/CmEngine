#include "UnTreatedRenderInformation.h"
#include "World.h"
#include "CameraActor.h"
#include "Canvas.h"
#include <array>

FUntreatedRenderInformation::FUntreatedRenderInformation(FCanvas _canvas, UWorld * _world, FCpuRenderFrameResource * _cpuRenderFrameResource)
{
	mCanvas = _canvas;
	mDeltaTime = GetDeltaTime();
	mTotalTime = GetTotalTime();

	auto & actors = _world->GetActors();

	new(&mUntreatedCamera) FUntreatedCamera(_world->GetCamera());
	new(&mUntreatedSkyBox) FUntreatedSkyBox(_world->GetSkyBox());

	for (auto _ele : actors)
	{
		switch (_ele->GetEntityType())
		{
		case EEntityType::SaticMesh:
			mUntreatedStaticMeshs.push_back(FUntreatedStaticMesh(dynamic_cast<AStaticMeshActor*>(_ele)));
			break;
		case EEntityType::DirectionLight:
			mUntreatedDirectionLights.push_back(FUntreatedDirectionLight(dynamic_cast<ADirectionLightActor*>(_ele)));
			break;
		case EEntityType::PontLight:
			mUntreatedPointLights.push_back(FUntreatedPointLight(dynamic_cast<APointLightActor*>(_ele)));
			break;
		case EEntityType::SpotLight:
			mUntreatedSpotLights.push_back(FUntreatedSpotLight(dynamic_cast<ASpotLightActor*>(_ele)));
			break;
		}
	}
}
