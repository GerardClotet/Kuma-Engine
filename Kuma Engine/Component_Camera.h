#ifndef _COMPONENT_CAMERA_H
#define _COMPONENT_CAMERA_H

#include "Components.h"
#include "GameObject.h"
#include "Color.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
class GameObject;

class Component_Camera : public Components
{
	public:
	Component_Camera(GameObject* game_object);
	Component_Camera();//SetLinked GameObjectManually
	~Component_Camera();

	void ReloadFrustrum();
	void DrawFrustum();
	void UpdateTransformFrustum();
	void DisplayInspector();
	void Look(const float3 &position);
	float3 GetCameraPosition() const;
	float* GetViewMatrix();
	float* GetProjectionMatrix() const;

	
	void SetAspectRatio(int width_ratio, int height_ratio, bool type = false);
	bool Update();

	void SaveScene(R_JSON_Value* val) const;

public:

	Frustum frustum;
	float* vertex;
	int index;
	int width, height;
	float aspect_ratio;

	float3 points[8];

	float vertical_fov = 60.0f;
	float horizontal_fov = 90.0f;

	float far_plane = 100.f;
	float near_plane = 1.0f;

	Color color_camera_bg{ 0.0f, 0.0f, 0.0f, 1.0f };
	Color color_frustum{ 1.0f, 0.54f, 0.0f, 1.0f };

	bool culling = false;
};


#endif // !_COMPONENT_CAMERA_H
