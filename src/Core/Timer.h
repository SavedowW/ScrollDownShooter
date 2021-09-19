#ifndef TIMER_H_
#define TIMER_H_
#include "Core.h"

class Timer
{
public:
	Timer();
	void begin();
	void getPassedMS();
	Uint32 m_lastFrameTime = 1;

private:
	Uint32 m_timeBegin = 0;
};

#endif