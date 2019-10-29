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

public:
	float3 position;
	float3 scale;
	Quat rotation;


};


#endif // !COMPONENT_TRANSFORM_H
