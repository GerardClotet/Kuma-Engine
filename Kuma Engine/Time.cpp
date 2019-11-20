#include "Time.h"

float Time::delta_time;
float Time::scale_time = 1;
float Time::time;

Timer Time::game_timer;

bool Time::paused = false;
bool Time::running = false;




void Time::Start()
{
	running = true;
	game_timer.Start();
	time = 0;
}

void Time::PreUpdate(float dt)
{
	//if game is running, give me dt, if not, give me 0
	delta_time = running ? dt : 0;

	if (running)
		time = game_timer.ReadTime()/1000.0f;
}

void Time::Update()
{
}


void Time::Play()
{
	//If we press Play button, start running
	running = true;
}

void Time::Pause()
{
	//if we press the pause button, stop the game_timer
	//We don't put running to false because we are not stopping game, we are just pausing it and it stills run
	paused = true;
	game_timer.Stop();
}

void Time::Resume()
{
	paused = false;
	game_timer.Resume();
}

void Time::Stop()
{
	//If we stop the game, return running=false
	running = false;
	game_timer.Stop();
}
