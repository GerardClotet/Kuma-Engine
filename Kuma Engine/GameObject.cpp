#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "Components.h"
#include "Component_Mesh.h"

GameObject::GameObject(std::string name, OBJECT_TYPE type)
{
	this->name = name;
	this->type = type;

	if (type == OBJECT_TYPE::CUBE)
	{
		//components.push_back(AddComponent(GO_COMPONENT::TRANSFORM));
		components.push_back(AddComponent(GO_COMPONENT::MESH));
	}
}



Components* GameObject::AddComponent(GO_COMPONENT type)
{
	Components* component = nullptr;
	switch (type)
	{
	case GO_COMPONENT::NONE:
		break;
	case GO_COMPONENT::MESH:
		component = new Component_Mesh(this->type);
		break;
	case GO_COMPONENT::TRANSFORM:
		break;
	case GO_COMPONENT::MATERIAL:
		break;
			
	}
	return component;
}

GameObject::~GameObject()
{
}

bool GameObject::Update()
{
	for (std::vector<Components*>::iterator item_comp = components.begin(); item_comp != components.end(); ++item_comp)
	{
		(*item_comp)->Update();
	}
	return true;
}
