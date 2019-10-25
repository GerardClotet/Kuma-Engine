#include "Component_Transform.h"
#include "Application.h"
#include "GameObject.h"

Component_Transform::Component_Transform(GameObject * obj)
{
	name = "transform";
	this->gameObject_Item = obj;
	comp_type = GO_COMPONENT::TRANSFORM;
}

Component_Transform::~Component_Transform()
{
	//remove transform
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
