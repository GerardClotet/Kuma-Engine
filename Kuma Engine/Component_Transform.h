#ifndef COMPONENT_TRANSFORM_H
#define COMPONENT_TRANSFORM_H

#include "Components.h"


class GameObject;

class Component_Transform :public Components
{
public:
	Component_Transform(GameObject* obj);
	~Component_Transform();


	bool Update();
	bool Enable();
	bool Disable();


public:
	GameObject* gameObject_Item = nullptr;


};


#endif // !COMPONENT_TRANSFORM_H
