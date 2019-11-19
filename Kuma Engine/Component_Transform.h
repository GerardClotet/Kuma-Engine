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
	const float3 GetGlobalPosition()const;

	//SCALE
	void SetLocalScale(const float x, const float y, const float z);
	const float3 GetLocalScale()const;
	const float3 GetGlobalScale()const;

	//LOADING SCENE

	//axis counter  x y z, 0 1 2
	void SetLocalPosToSingleAxis(const float pos, int axis);

	//axis ==3 sets Quat angle 
	void SetLocalRotToSingleAxis(const float pos, int axis);


	
	void SetLocalScaleToSingleAxis(const float scale, int axis);
	//ROTATION
	void SetLocalRotation(const float x, const float y, const float z, const float angle);
	const Quat GetLocalRotation()const;
	const Quat GetGlobalRotation()const;

	//MATRIX TRANSFORM
	void RecalculateTransformMatrix();
	float4x4 GetGlobalMatrix()const;

	float4x4 GetParentGlobalMatrix()const;
	void SaveScene(R_JSON_Value* val) const;

	bool ItIntersect(LineSegment ray);

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
	bool boundingBoxActive = false;


public:
	void DisplayInspector();

};


#endif // !COMPONENT_TRANSFORM_H
