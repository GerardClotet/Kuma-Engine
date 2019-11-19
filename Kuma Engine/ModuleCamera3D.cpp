#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "Component_Transform.h"
#include "Component_Camera.h"
#include "ImGui/imgui.h"
#include "RayCast.h"


ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	GameObject* camera_go = App->scene_intro->CreateGameObject(nullptr, OBJECT_TYPE::NONE, "Camera Fake");
	camera_go->AddComponent(GO_COMPONENT::TRANSFORM, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f,0.0f });
	camera_fake = (Component_Camera*)camera_go->AddComponent(GO_COMPONENT::CAMERA);
	camera_fake->frustum.nearPlaneDistance = 10.0f;
	camera_fake->frustum.farPlaneDistance = 500.0f;

	Reference = float3::zero;
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
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
			Focus();

		MovementCamera();
		ZoomCamera();

		if (!ImGuizmo::IsUsing() && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
			RotationCamera(dt);

		if (!ImGuizmo::IsUsing() && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
		{
			GameObject* pick_go = Pick();
			if (pick_go != nullptr)
				App->scene_intro->selected_game_obj = pick_go;

		}

		if (pickedRayCast)
			DrawLineSegment();

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
	if (!ImGuizmo::IsUsing() && App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		if (!capMouseInput)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);

			if (App->input->GetMouseXMotion() > 4) newPos -= frustum->WorldRight()*mouse_speed;
			if (App->input->GetMouseXMotion() < -4) newPos += frustum->WorldRight()*mouse_speed;

			if (App->input->GetMouseYMotion() < 5) newPos -= frustum->up*mouse_speed;
			if (App->input->GetMouseYMotion() > -5) newPos += frustum->up *mouse_speed;

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
			if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT) newPos += float3::unitY*speed;
			if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT) newPos -= float3::unitY*speed;

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += frustum->front*speed;

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= frustum->front*speed;


			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= frustum->WorldRight()*speed;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += frustum->WorldRight()*speed;
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
		int motion_x = App->input->GetMouseXMotion();
		int motion_y = App->input->GetMouseYMotion();

		if (motion_x != 0 || motion_y != 0)
		{
			float dx = (float)-motion_x * rot_speed * dt;
			float dy = (float)-motion_y * rot_speed * dt;

			Orbit(dx, dy);
		}
	}
}

void ModuleCamera3D::Focus()
{

}

void ModuleCamera3D::ZoomCamera()
{
	newPos = { 0,0,0 };
	if (!capMouseInput)
	{
		if (App->input->GetMouseWheel() > 0)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_ZoomIn);
			newPos += frustum->front * zoom_speed;
		}
		else if (App->input->GetMouseWheel() < 0)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_ZoomOut);
			newPos -= frustum->front * zoom_speed;
		}
		frustum->Translate(newPos * mouse_speed);

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{

			if (App->input->GetMouseYMotion() < 0)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ZoomIn);
				newPos += frustum->front * zoom_speed;
			}


			if (App->input->GetMouseYMotion() > 0)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ZoomOut);
				newPos -= frustum->front * zoom_speed;
			}

			if (App->input->GetMouseYMotion() == 0)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Zoom);
			}

			frustum->Translate(newPos * mouse_speed);
		}
	}
}

void ModuleCamera3D::Orbit(float motion_x, float motion_y)
{
	frustum->pos -= Reference;

	if (motion_x != 0)
	{
		vec3 rot_Y = rotate(vec3(frustum->up.x, frustum->up.y, frustum->up.z), motion_x, vec3(0.0f, 1.0f, 0.0f));
		vec3 rot_Z = rotate(vec3(frustum->front.x, frustum->front.y, frustum->front.z), motion_x, vec3(0.0f, 1.0f, 0.0f));

		frustum->up = float3(rot_Y.x, rot_Y.y, rot_Y.z);
		frustum->front = float3(rot_Z.x, rot_Z.y, rot_Z.z);
	}

	if (motion_y != 0)
	{
		vec3 rot_Y = rotate(vec3(frustum->up.x, frustum->up.y, frustum->up.z), motion_y, vec3(frustum->WorldRight().x, frustum->WorldRight().y, frustum->WorldRight().z));
		vec3 rot_Z = rotate(vec3(frustum->front.x, frustum->front.y, frustum->front.z), motion_y, vec3(frustum->WorldRight().x, frustum->WorldRight().y, frustum->WorldRight().z));

		frustum->up = float3(rot_Y.x, rot_Y.y, rot_Y.z);
		frustum->front = float3(rot_Z.x, rot_Z.y, rot_Z.z);

		if (frustum->up.y < 0.0f)
		{
			frustum->front = float3(0.0f, frustum->front.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			vec3 cross_vec = cross(vec3(frustum->front.x, frustum->front.y, frustum->front.z), vec3(frustum->WorldRight().x, frustum->WorldRight().y, frustum->WorldRight().z));
			frustum->up = float3(cross_vec.x, cross_vec.y, cross_vec.z);
		}
	}
	frustum->pos = Reference - frustum->front * length(vec3(frustum->pos.x, frustum->pos.y, frustum->pos.z));

	/*camera_fake->gameObject_Item->transform->RecalculateTransformMatrix();
	camera_fake->UpdateTransformFrustum();
	camera_fake->ReloadFrustrum();*/
	
}

void ModuleCamera3D::LookAt(float motion_x, float motion_y)
{
}

GameObject * ModuleCamera3D::Pick(float3 * hit_point)
{
	float width = (float)App->window->GetScreenWidth();
	float height = (float)App->window->GetScreenHeight();

	int mouse_x, mouse_y;
	mouse_x = App->input->GetMouseX();
	mouse_y = App->input->GetMouseY();
	
	float normalized_x = -(1.0f - (float(mouse_x) * 2.0f) / width);
	float normalized_y = 1.0f - (float(mouse_y) * 2.0f) / height;

	pick_ray = camera_fake->frustum.UnProjectLineSegment(normalized_x, normalized_y);

	RayCast ray;

	

	return App->scene_intro->MyRayCastIntersection(&pick_ray, ray);
}

void ModuleCamera3D::DrawLineSegment()
{
	glLineWidth(2.0f);
	glBegin(GL_LINES);

	glColor3f(ray_color.r, ray_color.g, ray_color.b);
	glVertex3f(pick_ray.a.x, pick_ray.a.y, pick_ray.a.z);
	glVertex3f(pick_ray.b.x, pick_ray.b.y, pick_ray.b.z);
	glEnd();
}

