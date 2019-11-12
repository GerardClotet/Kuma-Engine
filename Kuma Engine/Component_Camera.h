#ifndef _COMPONENT_CAMERA_H
#define _COMPONENT_CAMERA_H

#include "Components.h"
#include "GameObject.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
class GameObject;

class Component_Camera : public Components
{
	public:
	Component_Camera(GameObject* game_object);
	~Component_Camera();

	void ReloadFrustrum();
	Frustum frustum;
	void DrawFrustum();
	void UpdateTransformFrustum();
	void DisplayInspector();
	float* vertex;
	int index;
	void SetAspectRatio(int width_ratio, int height_ratio, bool type = false);
	bool Update();
	int width, height;
	float aspect_ratio;

	float3 points[8];

public:
	float vertical_fov = 60.0f;
	float horizontal_fov = 90.0f;

	float far_plane = 100.f;
	float near_plane = 0.1f;
};


#endif // !_COMPONENT_CAMERA_H
