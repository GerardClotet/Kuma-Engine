#ifndef TIME_H
#define TIME_H

#include "Timer.h"


class Time
{
public:

	//basic
	static void Start();
	static void PreUpdate(float dt);
	static void Update();

	//Buttons functions
	static void Play();
	static void Pause();
	static void Resume();
	static void Stop();

public:

	static float delta_time;
	static float scale_time;
	static float time;
	static Timer game_timer;
	static bool paused;
	static bool running;
	static bool play_one;
};


#endif // !TIME_H
