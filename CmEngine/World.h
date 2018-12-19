#pragma once
#include <vector>

class ACameraActor;
class AStaticMeshActor;
class ADirectionLightActor;
class APointLightActor;
class ASpotLightActor;

class UWorld
{
public:
	void BeginPlay();
	void Tick(float _deltaTime);
	void EndPlay();

	const ACameraActor * GetCamera()const { return mCamera; }
	const std::vector<AStaticMeshActor*>& GetStaticMeshs()const { return mStaticMeshActors; }
	const std::vector<ADirectionLightActor*>& GetDirectionLights()const { return mDirectionLights; }
	const std::vector<APointLightActor*>& GetPointLights()const { return mPointLights; }
	const std::vector<ASpotLightActor*>& GetSpotLights()const { return mSpotLights; }

private:
	ACameraActor * mCamera;
	std::vector<AStaticMeshActor*> mStaticMeshActors;
	std::vector<ADirectionLightActor*> mDirectionLights;
	std::vector<APointLightActor*> mPointLights;
	std::vector<ASpotLightActor*> mSpotLights;
};