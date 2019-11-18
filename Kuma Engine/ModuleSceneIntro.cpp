#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include <random>
#include <gl/GL.h>
#include "pcg-cpp-0.98/include/pcg_random.hpp"
#include "ModuleImporter.h"
#include "ModuleUI.h"
#include "ModuleCamera3D.h"
#include "ModuleFileSystem.h"
#include "ModuleRenderer3D.h"
#include "FBO.h"
#include "Component_Camera.h"
#include "Component_Mesh.h"
#include "Component_Transform.h"
#include "PanelConfig.h"
#include "Assimp/include/anim.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

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

	App->renderer3D->fbo->Bind();
	UpdateGameObject(root);
	App->renderer3D->fbo->Unbind();

	/*GuizmosControls();
	GuizmosLogic();*/

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
	GameObject* selected = nullptr;
	for (std::vector<RayCast>::iterator iter = scene_obj.begin(); iter != scene_obj.end(); ++iter)
	{
		temp = (*iter).trans->gameObject_Item;
		selected = TriangleTest(*ray, temp);
		if (selected != nullptr)
			break;
	}
	
	return selected;
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

GameObject* ModuleSceneIntro::TriangleTest(LineSegment& ray, GameObject* obj)
{
	bool intersected = false;
	Component_Mesh* mesh_go = nullptr;
	if (obj->hasComponent(GO_COMPONENT::MESH))
		mesh_go = obj->mesh;
	// TODO: if obj doesnt have mesh, just set it selected because it might be camera or light
	if (mesh_go != nullptr)
	{
		Component_Transform* trans_go = nullptr;
		if (obj->hasComponent(GO_COMPONENT::TRANSFORM))
			trans_go = obj->transform;

		for (uint i = 0; i < mesh_go->num_index; i += 3)
		{
			uint index_a, index_b, index_c;

			index_a = mesh_go->index[i] * 3;
			float3 point_a(&mesh_go->vertex[index_a]);

			index_b = mesh_go->index[i + 1] * 3;
			float3 point_b(&mesh_go->vertex[index_b]);

			index_c = mesh_go->index[i + 2] * 3;
			float3 point_c(&mesh_go->vertex[index_c]);

			Triangle triangle_to_check(point_a, point_b, point_c);
			triangle_to_check.Transform(trans_go->global_transformation);
			if (ray.Intersects(triangle_to_check, nullptr, nullptr))
			{
				LOG("DID IT");
				intersected = true;
				return obj;
				break;
			}
		}
		if (!intersected)
			return nullptr;

	}

	else if (obj->game_object_childs.empty())
		return obj;
	
	else
		return nullptr;
}

void ModuleSceneIntro::GuizmosControls()
{
	if ((App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN))
		guizmo_operation = ImGuizmo::OPERATION::TRANSLATE;

	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		guizmo_operation = ImGuizmo::OPERATION::ROTATE;

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		guizmo_operation = ImGuizmo::OPERATION::SCALE;

	if ((App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN))
		guizmo_mode = ImGuizmo::MODE::WORLD;

	if ((App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN))
		guizmo_mode = ImGuizmo::MODE::LOCAL;
}

void ModuleSceneIntro::GuizmosLogic()
{
	if (App->scene_intro->selected_game_obj != nullptr) {
		Component_Transform* transform = App->scene_intro->selected_game_obj->transform;

		float4x4 view_transposed = App->camera->camera_fake->frustum.ViewMatrix();
		view_transposed.Transpose();
		float4x4 projection_transposed = App->camera->camera_fake->frustum.ProjectionMatrix();
		projection_transposed.Transpose();
		float4x4 object_transform_matrix = transform->global_transformation;
		object_transform_matrix.Transpose();
		float4x4 delta_matrix;

		ImGuizmo::SetRect(0.0f, 0.0f, App->window->GetScreenWidth(), App->window->GetScreenHeight());
		ImGuizmo::SetDrawlist();
		ImGuizmo::Manipulate(view_transposed.ptr(), projection_transposed.ptr(), guizmo_operation, guizmo_mode, object_transform_matrix.ptr(), delta_matrix.ptr());
		ImGuizmo::Enable(true);
		if (ImGuizmo::IsUsing() && !delta_matrix.IsIdentity()/*Test if the gameobject is static or dynamic*/)
		{
			transform->SetLocalTransform(object_transform_matrix.Transposed());
		}
	}
	
}










