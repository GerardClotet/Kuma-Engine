#ifndef COMPONENT_TRANSFORM_H
#define COMPONENT_TRANSFORM_H

#include "Components.h"
#include "MathGeoLib/include/MathGeoLib.h"


class GameObject;

class Component_Transform :public Components
{
public:
	Component_Transform(GameObject* obj, float3 pos, float3 scale, Quat rot);
	~Component_Transform();


	bool Update();
	bool Enable();
	bool Disable();

private:
	//position
	float3 local_position = { 0,0,0 };
	// sacale
	float3 local_scale = { 0,0,0 };
	// rotation
	Quat local_rotation = { 0,0,0,0 };

	//euler angles
	float3 euler_rotation = { 0,0,0 };

public:

	float4x4 local_transformation = float4x4::identity;
	float4x4 global_transformation = float4x4::identity;

};


#endif // !COMPONENT_TRANSFORM_H
