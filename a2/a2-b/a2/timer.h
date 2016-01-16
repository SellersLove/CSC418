/***********************************************************
             CSC418, FALL 2009
 
                 timer.h
                 author: Mike Pratscher

		Timer class
			Provides a timer to drive the animation based
			on time rather than frame rate.

***********************************************************/

#ifndef __TIMER_H__
#define __TIMER_H__

#include <time.h>


class Timer
{
public:

	// constructor
	Timer() { reset(); }

	// destructor
	virtual ~Timer() {}

	// start the timer
	void reset() { startTime = clock(); }

	// query elapsed time
	double elapsed() const { return (double(clock() - startTime) / double(CLOCKS_PER_SEC)); }

private:

	clock_t startTime;

};

#endif // __TIMER_H__
