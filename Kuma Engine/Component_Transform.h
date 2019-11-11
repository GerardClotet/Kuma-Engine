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

	//POSITION
	void SetLocalPosition(const float x, const float y, const float z);
	float3& GetLocalPosition();
	const float3 GetGlobalPosition();

	//SCALE
	void SetLocalScale(const float x, const float y, const float z);
	const float3 GetLocalScale();
	const float3 GetGlobalScale();

	//ROTATION
	void SetLocalRotation(const float x, const float y, const float z, const float angle);
	const Quat GetLocalRotation();
	const Quat GetGlobalRotation();

	//MATRIX TRANSFORM
	void RecalculateTransformMatrix();
	float4x4 GetGlobalMatrix();

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


public:
	void DisplayInspector();

};


#endif // !COMPONENT_TRANSFORM_H
