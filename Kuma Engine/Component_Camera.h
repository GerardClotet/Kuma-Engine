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
	float* vertex;
	int index;
	void SetAspectRatio();
	bool Update();
	int width, height;
	float aspect_ratio;

	float3 points[8];
};


#endif // !_COMPONENT_CAMERA_H
