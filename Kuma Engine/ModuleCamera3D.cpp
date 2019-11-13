#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "Component_Transform.h"
#include "Component_Camera.h"
#include "ImGui/imgui.h"


ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	GameObject* camera_go = App->scene_intro->CreateGameObject(nullptr, OBJECT_TYPE::NONE, "Camera Fake");
	camera_go->AddComponent(GO_COMPONENT::TRANSFORM, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f,0.0f });
	camera_fake = (Component_Camera*)camera_go->AddComponent(GO_COMPONENT::CAMERA);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	App->saveLog("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	frustum = &camera_fake->frustum;
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	newPos = { 0,0,0 };
	speed = key_speed * dt;
	mouse_speed = camera_speed * dt;
	zoom_speed = camera_zoom_speed * dt;

	if (start_lerp)
	{
		camera_fake->frustum.pos = Lerp(camera_fake->frustum.pos, point_to_look, 0.5f);
		if (camera_fake->frustum.pos.Equals(point_to_look))
		{
			start_lerp = false;
		}
	}
	else
	{

		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		{
			speed = key_speed * dt * 2.0f;
			mouse_speed = camera_speed * dt * 2.0f;
			zoom_speed = camera_zoom_speed * dt * 2.0f;
		}


		//Focus
		/*if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			if (App->scene_intro->selected_game_obj != nullptr)
			{
				vec3 spot = { App->scene_intro->selected_game_obj->game_object_pos.x,
								App->scene_intro->selected_game_obj->game_object_pos.y,
								App->scene_intro->selected_game_obj->game_object_pos.z };
				LookAt(spot);
			}
		}*/

		MovementCamera();
		ZoomCamera();

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
			RotationCamera(dt);

	}
	return UPDATE_CONTINUE;
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3 &Movement)
{
	frustum->Translate(Movement);
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

void ModuleCamera3D::MovementCamera()
{
	newPos = { 0,0,0 };
	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		if (!capMouseInput)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);

			if (App->input->GetMouseXMotion() > 4) newPos -= frustum->WorldRight();
			if (App->input->GetMouseXMotion() < -4) newPos += frustum->WorldRight();

			if (App->input->GetMouseYMotion() < 5) newPos -= Y * mouse_speed;
			if (App->input->GetMouseYMotion() > -5) newPos += Y * mouse_speed;

			if (!newPos.Equals(float3::zero))
			{
				frustum->Translate(newPos * mouse_speed);
				Reference += newPos * mouse_speed;
			}
		}
	}
	else
	{
		if (!capMouseInput)
		{
			if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT) newPos += float3::unitY;
			if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT) newPos -= float3::unitY;

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += frustum->front;

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= frustum->front;


			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= frustum->WorldRight();
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += frustum->WorldRight();
		}

		if (!newPos.Equals(float3::zero))
		{
			frustum->Translate(newPos * speed);
			Reference += newPos * speed;

		}
	}
}

void ModuleCamera3D::RotationCamera(float dt)
{
	if (!capMouseInput)
	{
			float3 distance = camera_fake->frustum.pos - Reference;

			Quat rotationy(camera_fake->frustum.up, -App->input->GetMouseXMotion()* dt*0.3F);
			Quat rotationx(camera_fake->frustum.WorldRight(), -App->input->GetMouseYMotion()* dt*0.3F);

			distance = rotationx.Transform(distance);
			distance = rotationy.Transform(distance);

			camera_fake->frustum.pos = distance + Reference;

			camera_fake->Look(Reference);
	}
}

void ModuleCamera3D::ZoomCamera()
{
	newPos = { 0,0,0 };
	if (!capMouseInput)
	{
		if (App->input->GetMouseWheel() > 0)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_ZoomIn);
			newPos += frustum->front;
		}
		else if (App->input->GetMouseWheel() < 0)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_ZoomOut);
			newPos -= frustum->front;
		}
		frustum->Translate(newPos * mouse_speed);

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{

			if (App->input->GetMouseYMotion() < 0)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ZoomIn);
				newPos += frustum->front;
			}


			if (App->input->GetMouseYMotion() > 0)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ZoomOut);
				newPos -= frustum->front;
			}

			if (App->input->GetMouseYMotion() == 0)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Zoom);
			}

			frustum->Translate(newPos * mouse_speed);
		}
	}
}

