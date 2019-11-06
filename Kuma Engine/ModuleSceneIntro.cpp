#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include <random>
#include <gl/GL.h>
#include "pcg-cpp-0.98/include/pcg_random.hpp"
#include "ModuleImporter.h"
#include "ModuleUI.h"
#include "ModuleCamera3D.h"
#include "PanelConfig.h"


#include "GameObject.h"

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

	//RandomFloatGenerator();
	//RandomintGenerator(5, 6);
	//
	return ret;
}

bool ModuleSceneIntro::Start()
{

	root = new GameObject(nullptr,OBJECT_TYPE::PARENT, "root"); //empty gameobject containig  all game objects
	/*App->importer->LoadGeometry(firstFbx.c_str());
	App->importer->LoadImportedMaterials(firstTex);*/

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
	if (ImGui::IsAnyWindowHovered())
		App->camera->capMouseInput = true;
	else
		App->camera->capMouseInput = false;

	UpdateGameObject(root);

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	createGrid();

	//glColor3f(255.0f, 255.0f, 255.0f);
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::LoadConfig(JSON_Object *& config)
{
	firstFbx = json_object_dotget_string(config, "Importer.File");
	firstTex = json_object_dotget_string(config, "Importer.Texture");
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

void ModuleSceneIntro::createDirtyCube()
{
	//cube
	glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, ImageName);

	glBegin(GL_TRIANGLES);



	float lenght = 1;
	//Down
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1 * lenght, 0, 1 * lenght);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 0, 1 * lenght);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1 * lenght, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1 * lenght, 0, 1 * lenght);

	//Up
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 1 * lenght, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 1 * lenght, 1 * lenght);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1 * lenght, 1 * lenght, 1 * lenght);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 1 * lenght, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1 * lenght, 1 * lenght, 1 * lenght);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1 * lenght, 1 * lenght, 0);

	//Right
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 1 * lenght);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1 * lenght, 0, 1 * lenght);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 1 * lenght, 1 * lenght);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(1 * lenght, 0, 1 * lenght);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1 * lenght, 1 * lenght, 1 * lenght);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 1 * lenght, 1 * lenght);

	//Left
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 1 * lenght, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1 * lenght, 0, 0);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 1 * lenght, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1 * lenght, 1 * lenght, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1 * lenght, 0, 0);

	//Front
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 0, 1 * lenght);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 1 * lenght, 1 * lenght);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 1 * lenght, 1 * lenght);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 1 * lenght, 0);

	//Back
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1 * lenght, 0, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1 * lenght, 1 * lenght, 1 * lenght);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1 * lenght, 0, 1 * lenght);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(1 * lenght, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1 * lenght, 1 * lenght, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1 * lenght, 1 * lenght, 1 * lenght);

	glBindTexture(GL_TEXTURE_2D, 0);

	glEnd();

	glDisable(GL_TEXTURE_2D);

	glEnd();
	
}



GameObject* ModuleSceneIntro::CreateGameObject(GameObject* parent,OBJECT_TYPE type, std::string name)
{
	if(parent == nullptr)
		parent = root;
	
	
	GameObject* game_obj = new GameObject(parent,type,name);
	return game_obj;
}

void ModuleSceneIntro::UpdateGameObject(GameObject* parent)
{
	parent->Update();
	std::vector<GameObject*>::iterator iter = parent->game_object_childs.begin();
	for (iter; iter != parent->game_object_childs.end(); ++iter)
	{

		if (!(*iter)->show)
			UpdateGameObject((*iter));
	}
}







