#pragma once
#include <vector>
#include <thread>
#include <mutex>

class ACameraActor;
class ASkyBoxActor;
class AActor;

class UWorld
{
public:
	void BeginPlay();
	void Tick(float _deltaTime);
	void EndPlay();

	const ACameraActor * GetCamera()const { return mCamera; }
	const ASkyBoxActor * GetSkyBox()const { return mSkyBox; }
	const std::vector<AActor*>& GetActors()const { return mActors; }

private:
	std::mutex mDataMutex;
	std::thread mRecvThread;
	float mLastPosition[3] = {};
	float mLastRotation[3] = {};

	ACameraActor * mCamera;
	ASkyBoxActor * mSkyBox;
	std::vector<AActor*> mActors;
};
