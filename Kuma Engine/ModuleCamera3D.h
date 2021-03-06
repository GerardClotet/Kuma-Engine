#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "MathGeoLib/include/Geometry/LineSegment.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
#include "Color.h"

class Component_Camera;
class GameObject;


class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Move(const float3 &Movement);
	float* GetViewMatrix();
	void MovementCamera();
	void RotationCamera(float dt);
	void Focus();
	void ZoomCamera();
	void Orbit(float motion_x, float motion_y);
	void LookAt(float motion_x, float motion_y);
	GameObject* Pick(float3* hit_point = nullptr);
	void DrawLineSegment();


public:
	
	float camera_speed = 40.0f;
	float camera_zoom_speed = 60.0f;
	float key_speed = 35.0f;
	float rot_speed = 15.0f;
	float3 X, Y, Z, Position, Reference;
	bool capMouseInput	= false;

	Component_Camera* camera_fake = nullptr;
	Frustum* frustum = nullptr;
	LineSegment pick_ray;
	bool pickedRayCast = false;
	Color ray_color{ 1.0f, 0.0f, 0.0f, 1.0f };

private:
	bool start_lerp = false;
	float3 newPos{ 0,0,0 };
	float speed = 0.f;
	float zoom_speed = 0.f;
	float mouse_speed = 0.f;
	float3 point_to_look = (float3::zero);
	mat4x4 ViewMatrix, ViewMatrixInverse;
};