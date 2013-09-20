/**
 * file: CpuTimer.h
 * desc: A High Performance timer that measures the time spend on the CPU.
 *
 * Copyright 2013 Okke Schrijvers.
 */

#ifndef _CPU_TIMER_H_
#define _CPU_TIMER_H_

#include <stdio.h>

#ifdef _WIN32

#include <windows.h>

class CpuTimer
{
private:
	double msPerCount;
	unsigned long long prevTime;
	unsigned long long currTime;
	bool           started;

	// Constructor
public:
	CpuTimer()
	: started(false)
	{
		unsigned long long countsPerSec;
		QueryPerformanceFrequency((PLARGE_INTEGER)&countsPerSec);
		msPerCount = 1000.0 / (double)countsPerSec;
	}

	// Functions
public:
	void Start()
	{
		QueryPerformanceCounter((PLARGE_INTEGER)&prevTime);
		started = true;
	}

	double Stop()
	{
		if(started)
		{
			started = false;
			QueryPerformanceCounter((PLARGE_INTEGER)&currTime);
			return (currTime - prevTime)*msPerCount;
		}
		else
		{
			printf("Timer was not started.\n");
		}

		return -1.0;
	}
};

#else // use linux timer

#include <sys/time.h>

class CpuTimer
{
private:
	struct timeval startTime;
	struct timeval endTime;
	bool           started;

public:

	CpuTimer()
	: started(false)
	{}

	void Start()
	{
		gettimeofday(&startTime, NULL);
		started = true;
	}

	double Stop()
	{
		if(started)
		{
			started = false;
		    gettimeofday(&endTime, NULL);
			return (endTime.tv_sec - startTime.tv_sec)
					+ 1e-6 * (endTime.tv_usec - startTime.tv_usec);
		}
		else
		{
			printf("Timer was not started.\n");
		}

		return -1.0;
	}
};


#endif
#endif