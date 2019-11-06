#include "Component_Transform.h"
#include "Application.h"
#include "GameObject.h"

Component_Transform::Component_Transform(GameObject * obj, float3 pos, float3 scale, Quat rot)
{
	name = "transform";
	this->gameObject_Item = obj;
	this->local_position = pos;
	this->local_scale = scale;
	this->local_rotation = rot;

	//pass the quaternion to euler angles
	this->euler_rotation = this->local_rotation.ToEulerXYZ();
	//pass the euler angles from radiants to degrees
	this->euler_rotation.x = RadToDeg(this->euler_rotation.x);
	this->euler_rotation.y = RadToDeg(this->euler_rotation.y);
	this->euler_rotation.z = RadToDeg(this->euler_rotation.z);

	//initialize local transformation
	local_transformation = float4x4::FromTRS(this->local_position, this->local_rotation, this->local_scale);

	//Check if this game object has a parent. If it has, it's local transformation depends on it's parent global transform
	if (gameObject_Item->parent != nullptr)
	{
		if (gameObject_Item->parent->hasComponent(GO_COMPONENT::TRANSFORM))
			global_transformation = gameObject_Item->parent->transform->global_transformation * local_transformation;
		
		//If the parent exists but it has no Transform Component.
		else
			global_transformation = local_transformation;
		
	}
	//If the game object has no parent
	else
		global_transformation = local_transformation;
	

	comp_type = GO_COMPONENT::TRANSFORM;
}

Component_Transform::~Component_Transform()
{
	LOG("deleted transform");
	gameObject_Item = nullptr;
}

bool Component_Transform::Update()
{
	return true;
}

bool Component_Transform::Enable()
{
	return true;
}

bool Component_Transform::Disable()
{
	return true;
}
