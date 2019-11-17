#ifndef RAYCAST_H
#define RAYCAST_H

#include "Components.h"
class Component_Transform;

struct RayCast
{
public:
	Component_Transform* trans;
	float3 h_point;
	float distance;


	RayCast() : trans(nullptr), h_point(float3::zero), distance(0)
	{};
	RayCast(Component_Transform* transform) : trans(transform), h_point(float3::zero), distance(0)
	{};



};

#endif // !RAYCAST_H