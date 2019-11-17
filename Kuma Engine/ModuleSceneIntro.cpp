#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include <random>
#include <gl/GL.h>
#include "pcg-cpp-0.98/include/pcg_random.hpp"
#include "ModuleImporter.h"
#include "ModuleUI.h"
#include "ModuleCamera3D.h"
#include "ModuleFileSystem.h"
#include "Component_Camera.h"
#include "Component_Transform.h"
#include "PanelConfig.h"
#include "Assimp/include/anim.h"

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
	
	App->camera->camera_fake->frustum.pos = { -20,20,20 };
	App->camera->camera_fake->Look(float3(0, 0, 0));


	//RandomFloatGenerator();
	//RandomintGenerator(5, 6);
	//
	return ret;
}

bool ModuleSceneIntro::Start()
{

	root = new GameObject(nullptr,OBJECT_TYPE::PARENT, "root"); //empty gameobject containig  all game objects
	App->fs->ManageImportedFile(firstFbx.c_str());
	App->fs->ManageImportedFile(firstTex.c_str());


	GameObject* hard_camera_go = CreateGameObject(nullptr, OBJECT_TYPE::NONE, "Camera Harcoded");
	hard_camera_go->AddComponent(GO_COMPONENT::TRANSFORM, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f,0.0f });
	camera_hardcoded = (Component_Camera*)hard_camera_go->AddComponent(GO_COMPONENT::CAMERA);
	camera_hardcoded->frustum.farPlaneDistance = 30.0f;
	hard_camera_go->transform->SetLocalPosition(0.0f, 0.0f, 10.0f);
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


bool CompareRayCast(RayCast & a, RayCast & b)
{
	return a.distance < b.distance;
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

GameObject* ModuleSceneIntro::MyRayCastIntersection(LineSegment * ray, RayCast & hit)
{
	std::vector<RayCast> scene_obj;
	BoxIntersection(root, ray, scene_obj);

	//It takes the first value, and the last and with them two does the function compare
	std::sort(scene_obj.begin(), scene_obj.end(), CompareRayCast);
	GameObject* temp = nullptr;
	for (std::vector<RayCast>::iterator iter = scene_obj.begin(); iter != scene_obj.end(); ++iter)
	{
		temp = (*iter).trans->gameObject_Item;
		break;
	}
	
	return temp;
}

void ModuleSceneIntro::BoxIntersection(GameObject * obj, LineSegment * ray, std::vector<RayCast>& scene_obj)
{
	if (obj->hasComponent(GO_COMPONENT::TRANSFORM))
	{
		if (obj->transform->ItIntersect(*ray))
		{
			RayCast hit(obj->transform);

			float near_hit, far_hit;

			if (ray->Intersects(obj->bbox.obb, near_hit, far_hit))
			{
				hit.distance = near_hit;
				scene_obj.push_back(hit);
			}
		}
	}
		for (auto iter = obj->game_object_childs.begin(); iter != obj->game_object_childs.end(); ++iter)
		{
			BoxIntersection((*iter), ray, scene_obj);
		}

	

}

bool ModuleSceneIntro::TriangleTest(LineSegment * ray, std::vector<RayCast>& scene_obj, RayCast & point)
{
	return false;
}










