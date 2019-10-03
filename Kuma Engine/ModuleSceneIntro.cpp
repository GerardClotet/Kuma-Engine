#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include <random>
#include "RandomHelper.h"
#include <gl/GL.h>
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

bool ModuleSceneIntro::Start()
{
	

	return true;
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
	
	

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	createGrid();

	createSphere(vec3(-1, 2, -2), 1, { 255,0,0,255 }); //memory leak function 

	createCube(vec3(0, 0, 0), { 0,255,255,255 });
	createCube(vec3(5, 0, 0), { 255,0,255,255 });
	createCube(vec3(10, 0, 0), { 255,255,0,255 });
	

	glColor3f(255, 255, 255);
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	
}


void ModuleSceneIntro::createCube(const vec3 &position, Color color)
{
	glColor3f(color.r, color.g, color.b);

	par_shapes_mesh* cube = par_shapes_create_cube();

	par_shapes_translate(cube, position.x, position.y, position.z);

	uint my_id = 0;
	uint my_indices = 0;

	// buffer points
	glGenBuffers(1, &my_id);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cube->npoints * 3, cube->points, GL_STATIC_DRAW);

	// buffer index
	glGenBuffers(1, &my_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * cube->ntriangles * 3, cube->triangles, GL_STATIC_DRAW);


	//Draw
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, cube->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

	
}

void ModuleSceneIntro::createSphere(const vec3& position, int subdivisions, Color color)
{
	glColor3f(color.r, color.g, color.b);
	par_shapes_mesh* sphere = par_shapes_create_subdivided_sphere(subdivisions);
	par_shapes_translate(sphere, position.x, position.y, position.z);

	uint my_id = 0;
	uint my_indices = 0;

	// buffer points
	glGenBuffers(1, &my_id);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sphere->npoints * 3, sphere->points, GL_STATIC_DRAW);

	// buffer index
	glGenBuffers(1, &my_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * sphere->ntriangles * 3, sphere->triangles, GL_STATIC_DRAW);


	//Draw
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, sphere->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}


void ModuleSceneIntro::createGrid()
{
	glLineWidth(1.0f);
	glBegin(GL_LINES);

	for (int i = -max_grid; i <= max_grid; i++)
	{

		sunX = separator * i;
		

		glVertex3f(sunX, 0.0f, -sunZ);
		glVertex3f(sunX, 0, sunZ);


		sunQ = separator * i;
		

		glVertex3f(-sunW, 0.0f, sunQ);
		glVertex3f(sunW, 0, sunQ);
	}
	glEnd();
}



