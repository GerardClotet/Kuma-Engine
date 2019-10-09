#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Mesh.h"
#include <random>
#include "RandomHelper.h"
#include <gl/GL.h>
#include "pcg-cpp-0.98/include/pcg_random.hpp"
#include "ModuleImporter.h"
#include "ModuleUI.h"
#include "PanelConfig.h"
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
////	meshItem->createCube({ 100,100,100 }, { 30.0f,100.0f,86.0f });
//	Mesh meshStart;
//	meshStart.createCube({ 3,1,6 }, { 30.0f,100.0f,86.0f });
	glGenBuffers(1, (GLuint*) & (my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
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
	//Draw
	createGrid();

	/*for (std::list<Mesh*>::iterator item_mesh = mesh_list.begin(); item_mesh != mesh_list.end(); ++item_mesh)
	{
		(*item_mesh)->Render();
	}*/

	glEnableClientState(GL_VERTEX_ARRAY);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	glTexCoord2f(vertices);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// � draw other buffers
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableClientState(GL_VERTEX_ARRAY);


	glColor3f(255.0f, 255.0f, 255.0f);
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	
}







void ModuleSceneIntro::createGrid()
{
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	glColor4fv((float*)& ImVec4(1,1,1,1));
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

void ModuleSceneIntro::createSphere(const vec3& position, int subdivisions,  Color color)
{
	//glColor3f(color.r, color.g, color.b);

	//Meshes mesh;
	//par_shapes_mesh* sphere;
	//sphere = par_shapes_create_subdivided_sphere(subdivisions);


	//mesh.num_index = sphere->ntriangles;
	//mesh.index = sphere->triangles;
	//mesh.num_vertex = sphere->npoints;
	//mesh.vertex = sphere->points;

	//par_shapes_translate(sphere, position.x, position.y, position.z);

	//// buffer points
	//glGenBuffers(1, &mesh.id_vertex);
	//glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertex);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.num_vertex * 3, mesh.vertex, GL_STATIC_DRAW);

	//// buffer index
	//glGenBuffers(1, &mesh.id_index);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_index);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh.num_index * 3, mesh.index, GL_STATIC_DRAW);

	//mesh_list.push_back(mesh);
}



