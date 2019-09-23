#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include <random>



#include <gl/GL.h>

#include "MathGeoLib/include/Geometry/Plane.h"


#ifdef NDEBUG //no debug
#pragma comment (lib, "MathGeoLib/libx86/Release/MathGeoLib.lib") 
#else
#pragma comment (lib, "MathGeoLib/libx86/Debug/MathGeoLib.lib") 
#endif

#include "pcg-cpp-0.98/include/pcg_random.hpp"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	// Seed with a real random value, if available
	pcg_extras::seed_seq_from<std::random_device> seed_source;

	// Make a random number engine
	pcg32 rng(seed_source);

	// Choose a random mean between 1 and 6
	std::uniform_int_distribution<int> uniform_dist(1, 6);
	int mean = uniform_dist(rng);
	
	LOG("%i", mean);
	
	
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	//Plane p(0, 1, 0, 0);
	//p.axis = true;
	//p.Render();

	Sphere es ({ 0,0,0 }, 5);
	Sphere z({ 0,0,0 }, 3);
	if (z.Intersects(es))
	{
		LOG("intersected");
		Sphere ais({ 10,0,00 }, 500);
		
	}
	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}



