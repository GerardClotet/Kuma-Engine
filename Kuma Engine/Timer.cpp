// ----------------------------------------------------
// Timer.cpp
// Body for CPU Tick Timer class
// ----------------------------------------------------

#include "Timer.h"

// ---------------------------------------------
Timer::Timer()
{
	Start();
}

// ---------------------------------------------
void Timer::Start()
{
	running = true;
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Stop()
{
	running = false;
	stopped_at = SDL_GetTicks();
}

// ---------------------------------------------

void Timer::Resume()
{
	if (running == false)
	{
		running = true;
		started_at = SDL_GetTicks() - (stopped_at - started_at);
	}
}

// ---------------------------------------------
Uint32 Timer::ReadTime()
{
	if(running == true)
	{
		return SDL_GetTicks() - started_at;
	}
	else
	{
		return stopped_at - started_at;
	}
}

uint Timer::ReadSec()
{
	if (running == true)
	{
		return (SDL_GetTicks() - started_at)/1000.0f;
	}
	else
	{
		return (stopped_at - started_at)/1000.0f;
	}
}


