#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "ImGui/imgui.h"


ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 10.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
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
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	newPos = { 0,0,0 };
	speed = key_speed * dt;
	mouse_speed = camera_speed * dt;
	zoom_speed = camera_zoom_speed * dt;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		speed = key_speed * dt * 2.0f;
		mouse_speed = camera_speed * dt * 2.0f;
		zoom_speed = camera_zoom_speed * dt * 2.0f;
	}
	
	
	
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		if (App->scene_intro->selected_game_obj != nullptr)
		{
			vec3 spot = { App->scene_intro->selected_game_obj->game_object_pos.x,
							App->scene_intro->selected_game_obj->game_object_pos.y,
							App->scene_intro->selected_game_obj->game_object_pos.z };
			LookAt(spot);
		}
	}

	if (!capMouseInput)
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;
	}

	MovementCamera();
	ZoomCamera();

	Position += newPos;
	Reference += newPos;

	RotationCamera();

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

void ModuleCamera3D::MovementCamera()
{
	if (!capMouseInput)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);

			if (App->input->GetMouseXMotion() > 4) newPos -= X * mouse_speed;
			if (App->input->GetMouseXMotion() < -4) newPos += X * mouse_speed;

			if (App->input->GetMouseYMotion() < 5) newPos -= Y * mouse_speed;
			if (App->input->GetMouseYMotion() > -5) newPos += Y * mouse_speed;

		}
	}
}

void ModuleCamera3D::RotationCamera()
{
	if (!capMouseInput)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

			Position -= Reference;

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				Y = rotate(Y, DeltaY, X);
				Z = rotate(Z, DeltaY, X);

				if (Y.y < 0.0f)
				{
					Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					Y = cross(Z, X);
				}
			}

			Position = Reference + Z * length(Position);
		}
	}
}

void ModuleCamera3D::ZoomCamera()
{
	if (!capMouseInput)
	{
		if (App->input->GetMouseWheel() > 0)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_ZoomIn);
			newPos -= Z * zoom_speed;
		}
		else if (App->input->GetMouseWheel() < 0)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_ZoomOut);
			newPos += Z * zoom_speed;
		}

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{

			if (App->input->GetMouseYMotion() < 0)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ZoomIn);
				newPos -= Z * zoom_speed;
			}


			if (App->input->GetMouseYMotion() > 0)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ZoomOut);
				newPos += Z * zoom_speed;
			}

			if (App->input->GetMouseYMotion() == 0)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Zoom);
			}


		}
	}
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}