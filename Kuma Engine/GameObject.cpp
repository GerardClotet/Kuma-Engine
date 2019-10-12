#include "GameObject.h"

#include "ModuleSceneIntro.h"

GameObject::GameObject(std::string name, OBJECT_TYPE type)
{
	this->name = name;
	this->type = type;

	if (type == OBJECT_TYPE::CUBE)
	{
		
	}
}

GameObject::~GameObject()
{
}

bool GameObject::Update()
{
	return true;
}
