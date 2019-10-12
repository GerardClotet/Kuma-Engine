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
		
	}
}

//Components* GameObject::createComponents(COMP_TYPE type)
//{
//	switch (type)
//	{
//	case MESH:
//
//		break;
//
//	case NONE:
//
//		return nullptr;
//		break;
//	case TRANSFORM:
//		break;
//
//	case MATERIAL:
//		break;
//	default:
//		break;
//	}
//	switch(type)
//	return nullptr;
//}

Components* GameObject::AddComponent(GO_COMPONENT type)
{

	switch (type)
	{
	case GO_COMPONENT::NONE:
		break;
	case GO_COMPONENT::MESH:
		break;
	case GO_COMPONENT::TRANSFORM:
		break;
	case GO_COMPONENT::MATERIAL:
		break;
			
	}
	return nullptr;
}

GameObject::~GameObject()
{
}

bool GameObject::Update()
{
	return true;
}
