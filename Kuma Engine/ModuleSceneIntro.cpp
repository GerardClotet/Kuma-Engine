#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include <random>
#include "RandomHelper.h"
#include "par_shapes.h"

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

bool ModuleSceneIntro::Start()
{
	glGenBuffers(1, (GLuint*) & (my_id0));
	glBindBuffer(GL_ARRAY_BUFFER, my_id0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, vertices, GL_STATIC_DRAW);

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
	glLineWidth(2.0f);
	glBegin(GL_LINES);

	for (int i = -100; i <= 100; i++)
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
	glColor3f(255, 255, 0);
	glEnd();



	//cube
	//glBegin(GL_TRIANGLES);

	////front face
	//glVertex3f(0.0f, 0.0f, 0.0f);
	//glVertex3f(0.0f, 1.0f, 0.0f);
	//glVertex3f(1.0f,1.0f,0.0f);
	//

	//glVertex3f(1.0f, 1.0f, 0.0f);
	//glVertex3f(1.0f, 0.0f, 0.0f);
	//glVertex3f(0.0f, 0.0f, 0.0f);

	////back face
	//glVertex3f(0.0f, 0.0f, 1.0f); //v7
	//glVertex3f(1.0f, 0.0f, 1.0f); //v4
	//glVertex3f(1.0f, 1.0f, 1.0f); //v5

	//glVertex3f(1.0f, 1.0f, 1.0f); //v5
	//glVertex3f(0.0f, 1.0f, 1.0f); //v6
	//glVertex3f(0.0f, 0.0f, 1.0f); //v7


	////top face
	//glVertex3f(1.0f, 1.0f, 0.0f); //v0
	//glVertex3f(0.0f, 1.0f, 0.0f); //v1
	//glVertex3f(0.0f, 1.0f, 1.0f); //v6

	//glVertex3f(0.0f, 1.0f, 1.0f); //v6
	//glVertex3f(1.0f, 1.0f, 1.0f); //v5
	//glVertex3f(1.0f, 1.0f, 0.0f); //v0

	////bottom face

	//glVertex3f(1.0f, 0.0f, 0.0f); //v3
	//glVertex3f(1.0f, 0.0f, 1.0f); //v4
	//glVertex3f(0.0f, 0.0f, 1.0f); //v7

	//glVertex3f(1.0f, 0.0f, 0.0f); //v3
	//glVertex3f(0.0f, 0.0f, 1.0f); //v7
	//glVertex3f(0.0f, 0.0f, 0.0f); //v2

	////right face
	//glVertex3f(1.0f, 1.0f, 0.0f);
	//glVertex3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(1.0f, 0.0f, 1.0f);

	//glVertex3f(1.0f, 0.0f, 1.0f);
	//glVertex3f(1.0f, 0.0f, 0.0f);
	//glVertex3f(1.0f, 1.0f, 0.0f);



	//
	////left face

	//glVertex3f(0.0f, 1.0f, 0.0f); //v1
	//glVertex3f(0.0f, 0.0f, 0.0f); //v2
	//glVertex3f(0.0f, 0.0f, 1.0f); //v7

	//glVertex3f(0.0f, 0.0f, 1.0f); //v7
	//glVertex3f(0.0f, 1.0f, 1.0f); //v6
	//glVertex3f(0.0f, 1.0f, 0.0f); //v1



//	glEnd();

	//glBegin(GL_TRIANGLES);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id0);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// � draw other buffers
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableClientState(GL_VERTEX_ARRAY);


	//glEnd();


	float vertices2[] = {
					0.0f,0.0f,0.0f,
					1.0f,0.0f,0.0f,
					1.0f,1.0f,0.0f,
					0.0f,1.0f,0.0f,
					0.0f,1.0f,1.0f,
					0.0f,0.0f,1.0f,
					1.0f,0.0f,1.0f,
					1.0f,1.0f,1.0f
	};


	int indices[] = { 2,1,0, 0,3,2,  //front face // 36 of indices
					 4,3,0, 0,5,4,
					 6,5,0, 0,1,6,
					 7,6,1, 1,2,7,
					 3,4,7, 7,2,3,
					 6,7,4, 4,5,6 };


	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, vertices2);
	// � draw other buffers
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);
	glDisableClientState(GL_VERTEX_ARRAY);
	

	par_shapes_create_cylinder(50, 50);

	glColor3f(255, 255, 255);
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}



