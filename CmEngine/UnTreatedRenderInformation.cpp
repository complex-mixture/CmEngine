#include "UnTreatedRenderInformation.h"
#include "World.h"
#include "CameraActor.h"
#include "Canvas.h"

FUntreatedRenderInformation::FUntreatedRenderInformation(FCanvas _canvas, UWorld * _world, FCpuRenderFrameResource * _cpuRenderFrameResource)
{
	mCanvas = _canvas;
	mViewMatrix = _world->GetCamera()->GetViewMatrix();
	mEyePosition = _world->GetCamera()->GetPosition();
	mDeltaTime = GetDeltaTime();
	mTotalTime = GetTotalTime();

	auto staticMeshs = _world->GetStaticMeshs();
	mUntreatedRenderStaticMeshs.resize(staticMeshs.size());
	for (int i = 0; i != staticMeshs.size(); ++i)
		new(&mUntreatedRenderStaticMeshs[i]) FUntreatedRenderStaticMesh(staticMeshs[i]);

	auto directionLights = _world->GetDirectionLights();
	mUntreatedDirectionLights.resize(directionLights.size());
	for (int i = 0; i != directionLights.size(); ++i)
		new(&mUntreatedDirectionLights[i]) FUntreatedDirectionLight(directionLights[i]);

	auto pointLights = _world->GetPointLights();
	mUntreatedPointLights.resize(pointLights.size());
	for (int i = 0; i != pointLights.size(); ++i)
		new(&mUntreatedPointLights[i]) FUntreatedPointLight(pointLights[i]);

	auto spotLights = _world->GetSpotLights();
	mUntreatedSpotLights.resize(spotLights.size());
	for (int i = 0; i != spotLights.size(); ++i)
		new(&mUntreatedSpotLights[i]) FUntreatedSpotLight(spotLights[i]);

	Assert(directionLights.size() + pointLights.size() + spotLights.size() <= 255);
}