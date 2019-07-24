#pragma once
#ifndef CTIMER_HPP
#define CTIMER_HPP
#include <chrono>
#include <mutex>
#include <condition_variable>


	class CTimer
	{
	public:
		CTimer();
		CTimer(long timeout);
		CTimer(const long timeout, const uint8_t numberOfChecks);
		virtual ~CTimer();

		bool isDone();
		void setDone();
		void waitDone();
		bool isTimedOut();
		void reset(const long timeout);
		void reset(const long timeout, const uint8_t numberOfChecks);

	private:

		std::chrono::milliseconds	mTimeout;
		std::chrono::milliseconds	mStartTime;
		uint8_t						mNumberOfChecks;
		bool 						mDone;
		bool						mWaited;
		std::mutex					mMutex;
		std::mutex 					mCvMutex;
		std::condition_variable 	mCv;

	};

#endif


