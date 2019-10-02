#pragma once
#include "Module.h"
#include "Globals.h"



#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLibFwd.h"

#define MAX_SNAKE 2



class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Init();
	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
public:
	
	SDL_Window* window;

	uint my_id = 1;

private:
	uint my_id0 = 0;
	int num_vertices = 8;

	float vertices[36*3] = {
	0.0f,0.0f,0.0f,
	1.f,1.f,0.f,
	1.f,1.f,0.f,
	1,1,0.f,
	1,0,0,
	0,0,0,//6
	0,0,1,
	1,0,1,
	1,1,1,//9
	1,1,1,
	0,1,1,
	0,0,1,//12
	1,1,0,
	0,1,0,
	0,1,1,//15
	0,1,1,
	1,1,1,
	1,1,0,//18
	1,0,0,
	1,0,1,
	0,0,1,//21
	1,0,0,
	0,0,1,
	0,0,0,//24
	1,1,0,
	1,1,1,
	1,0,1,//27
	1,0,1,
	1,0,0,
	1,1,0,//30
	0,1,0,
	0,0,0,
	0,0,1,//33
	0,0,1,
	0,1,1,
	0,1,0//36
	};
	
};
