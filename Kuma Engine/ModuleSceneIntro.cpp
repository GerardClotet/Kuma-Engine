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

	/*glGenBuffers(1, (GLuint*) & (my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, vertices, GL_STATIC_DRAW);*/




	//v = CreateCubeVertices(vertex_num, 1);

	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
	checkImage;
	LOG("");
	

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &(GLuint)ImageName);
	glBindTexture(GL_TEXTURE_2D, (GLuint)ImageName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);


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

	

	//cube
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ImageName);

	glBegin(GL_TRIANGLES);



	//front face
	glVertex3f(0.0f, 0.0f, 0.0f);//v2
	glTexCoord2f(0.5, 0.33f);
	glVertex3f(0.0f, 1.0f, 0.0f); //v1
	glTexCoord2f(.25, 0.0f);
	glVertex3f(1.0f,1.0f,0.0f); //v0
	glTexCoord2f(0.25, 0.33f);


	glVertex3f(1.0f, 1.0f, 0.0f); //v0

	glTexCoord2f(.25, 0.0f);

	glVertex3f(1.0f, 0.0f, 0.0f); //v3
	glTexCoord2f(0.5, .33f);
	glVertex3f(0.0f, 0.0f, 0.0f); //v2
	glTexCoord2f(.5, 0.0f);

	//back face
	glVertex3f(0.0f, 0.0f, 1.0f); //v7
	glTexCoord2f(.25, 1.0f);

	glVertex3f(1.0f, 0.0f, 1.0f); //v4
	glTexCoord2f(.25, 0.66f);

	glVertex3f(1.0f, 1.0f, 1.0f); //v5
	glTexCoord2f(.5, 0.66f);

	glVertex3f(1.0f, 1.0f, 1.0f); //v5
	glTexCoord2f(.5, 0.66f);


	glVertex3f(0.0f, 1.0f, 1.0f); //v6

	glTexCoord2f(.5, 1.0f);

	glVertex3f(0.0f, 0.0f, 1.0f); //v7
	glTexCoord2f(.25, 1.0f);


	//top face
	glVertex3f(1.0f, 1.0f, 0.0f); //v0
	//glTexCoord2f(.5, 0.33f);
	glTexCoord2f(.25, 0.33f);
	glVertex3f(0.0f, 1.0f, 0.0f); //v1
	//glTexCoord2f(.25, 0.33f);
	glTexCoord2f(.25, 0.66f);
	glVertex3f(0.0f, 1.0f, 1.0f); //v6
	glTexCoord2f(.5, 0.33f);
//	glTexCoord2f(.25, 0.66f);


	glVertex3f(0.0f, 1.0f, 1.0f); //v6
	//glTexCoord2f(.25, 0.66f);
	glTexCoord2f(.5, 0.66f);
	glVertex3f(1.0f, 1.0f, 1.0f); //v5
//	glTexCoord2f(.5, 0.66f);
	glTexCoord2f(.5, 0.33f);
	glVertex3f(1.0f, 1.0f, 0.0f); //v0
	glTexCoord2f(.25, 0.66f);
//	glTexCoord2f(.5, 0.33f);

	//bottom face

	glVertex3f(1.0f, 0.0f, 0.0f); //v3
	//glTexCoord2f(.75, 0.33f);
	glTexCoord2f(.75, 0.66f);
	glVertex3f(1.0f, 0.0f, 1.0f); //v4
	//glTexCoord2f(.75, 0.66f);
	glTexCoord2f(1, 0.66f);
	glVertex3f(0.0f, 0.0f, 1.0f); //v7
	glTexCoord2f(.75, 0.33f);
	//glTexCoord2f(1, 0.66f);

	glVertex3f(1.0f, 0.0f, 0.0f); //v3
	glTexCoord2f(1, 0.66f);
	glVertex3f(0.0f, 0.0f, 1.0f); //v7
	glTexCoord2f(1, 0.33f);
	glVertex3f(0.0f, 0.0f, 0.0f); //v2
	glTexCoord2f(.75, 0.33f);

	//right face
	glVertex3f(1.0f, 1.0f, 0.0f); //v0
	glTexCoord2f(.5, 0.66f);

	glVertex3f(1.0f, 1.0f, 1.0f);//v5
	glTexCoord2f(.75, 0.66f);

	glVertex3f(1.0f, 0.0f, 1.0f);//v4
	glTexCoord2f(.5, 0.33f);



	glVertex3f(1.0f, 0.0f, 1.0f);//v4
	glTexCoord2f(.75, 0.33f);

	glVertex3f(1.0f, 0.0f, 0.0f);//v3
	glTexCoord2f(.5, 0.33f);

	glVertex3f(1.0f, 1.0f, 0.0f);//v0
	glTexCoord2f(.75, 0.66f);



	
	//left face

	glVertex3f(0.0f, 1.0f, 0.0f); //v1
	glTexCoord2f(.0, 0.33f);

	glVertex3f(0.0f, 0.0f, 0.0f); //v2
	glTexCoord2f(0, 0.66f);

	glVertex3f(0.0f, 0.0f, 1.0f); //v7
	glTexCoord2f(.25, 0.33f);


	glVertex3f(0.0f, 0.0f, 1.0f); //v7
	glTexCoord2f(0, 0.66f);

	glVertex3f(0.0f, 1.0f, 1.0f); //v6
	glTexCoord2f(.25, 0.66f);

	glVertex3f(0.0f, 1.0f, 0.0f); //v1
	glTexCoord2f(.25, 0.33f);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);

	glEnd();


		//Draw
	createGrid();
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








