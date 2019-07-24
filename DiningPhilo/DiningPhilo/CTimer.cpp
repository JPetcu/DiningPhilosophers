#include "stdafx.h"
#include <thread>
#include <iostream>
#include "CTimer.h"
using namespace std::chrono;

CTimer::CTimer() : mTimeout(std::chrono::milliseconds(0))
, mStartTime(duration_cast<milliseconds>(system_clock::now().time_since_epoch()))
, mNumberOfChecks(0)
, mDone(true)
, mWaited(false)
, mMutex()
, mCvMutex()
, mCv()
{

}

CTimer::CTimer(const long timeout) : mTimeout(std::chrono::milliseconds(timeout))
, mStartTime(duration_cast<milliseconds>(system_clock::now().time_since_epoch()))
, mNumberOfChecks(1)
, mDone(false)
, mWaited(false)
, mMutex()
, mCvMutex()
, mCv()
{

}

CTimer::CTimer(const long timeout, const uint8_t numberOfChecks) : mTimeout(std::chrono::milliseconds(timeout))
, mStartTime(duration_cast<milliseconds>(system_clock::now().time_since_epoch()))
, mNumberOfChecks(numberOfChecks)
, mDone(false)
, mWaited(false)
, mMutex()
, mCvMutex()
, mCv()
{

}

void CTimer::reset(const long timeout)
{
	mTimeout = std::chrono::milliseconds(timeout);
	mNumberOfChecks = 1;
	mDone = false;
	mWaited = false;
}

void CTimer::reset(const long timeout, const uint8_t numberOfChecks)
{
	mTimeout = std::chrono::milliseconds(timeout);
	mNumberOfChecks = numberOfChecks;
	mDone = false;
	mWaited = false;
}

CTimer::~CTimer()
{

}

bool CTimer::isDone()
{
	std::lock_guard<std::mutex> lock(mMutex);
	return mDone;
}

void CTimer::setDone()
{
	//decrement (saturate to 0)
	{
		std::lock_guard<std::mutex> lock(mMutex);
		if (mNumberOfChecks)
		{
			mNumberOfChecks--;
		}
	}
	if (0 == mNumberOfChecks)
	{
		std::unique_lock<std::mutex> lk(mCvMutex);
		lk.unlock();
		mCv.notify_one();
	}
}

void CTimer::waitDone()
{
	{
		std::lock_guard<std::mutex> lock(mMutex);
		mWaited = true;
	}
	std::unique_lock<std::mutex> lk(mCvMutex);
	mCv.wait_for(lk, std::chrono::milliseconds(mTimeout), [&] {return !isTimedOut(); });
	std::lock_guard<std::mutex> lock(mMutex);
	mDone = true;
}

bool CTimer::isTimedOut()
{
	std::lock_guard<std::mutex> lock(mMutex);
	if (mWaited)
	{
		return 0 != mNumberOfChecks;
	}
	else
	{
		return (mStartTime + mTimeout < duration_cast<milliseconds>(system_clock::now().time_since_epoch()));
	}

}
