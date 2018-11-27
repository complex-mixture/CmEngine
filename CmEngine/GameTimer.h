#pragma once
#include <stdint.h>

class FGameTimer
{
public:
	FGameTimer();

	float TotalTime()const; // in seconds
	float DeltaTime()const; // in seconds

	void Reset(); // Call before message loop.
	void Start(); // Call when unpaused.
	void Stop();  // Call when paused.
	void Tick();  // Call every frame.

private:
	double mSecondsPerCount;
	double mDeltaTime;

	int64_t mBaseTime;
	int64_t mPausedTime;
	int64_t mStopTime;
	int64_t mPrevTime;
	int64_t mCurrTime;

	bool mStopped;
};
