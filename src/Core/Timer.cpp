#include "Timer.h"

Timer::Timer()
{
}

void Timer::begin()
{
	m_timeBegin = SDL_GetTicks();
}

void Timer::getPassedMS()
{
	Uint32 timeEnd = SDL_GetTicks();
	m_lastFrameTime = Uint32(timeEnd - m_timeBegin);
	begin();
}
