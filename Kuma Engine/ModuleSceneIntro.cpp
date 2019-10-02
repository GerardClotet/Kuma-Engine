#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include <random>
#include "RandomHelper.h"


#include <gl/GL.h>

#include "MathGeoLib/include/Geometry/Plane.h"


//#ifdef NDEBUG //no debug
//#pragma comment (lib, "MathGeoLib/libx86/Release/MathGeoLib.lib") 
//#else
//#pragma comment (lib, "MathGeoLib/libx86/Debug/MathGeoLib.lib") 
//#endif

#include "pcg-cpp-0.98/include/pcg_random.hpp"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Init()
{
	App->saveLog("Loading Intro assets");
	bool ret = true;
	
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	RandomFloatGenerator();
	RandomintGenerator(5, 6);
	

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
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	
	for (int i=-100; i <= 100; i++)
	{
	
		float sunX = 2 * i;
		float sunZ = 150;

		glVertex3f(sunX, 0.0f, -150.0f);
		glVertex3f(sunX, 0, sunZ);


		float sunQ = 2 * i;
		float sunW = 150;

		glVertex3f(-150.0f, 0.0f, sunQ);
		glVertex3f(sunW, 0, sunQ);
	}

	glEnd();
	
	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	Plane p({ 0,1,0 }, 0);


	Sphere es({ 0,0,0 }, 5);
	Sphere z({ 0,0,0 }, 3);
	if (z.Intersects(es))
	{
		//LOG("intersected");
		Sphere ais({ 10,0,00 }, 500);

	}

	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}



