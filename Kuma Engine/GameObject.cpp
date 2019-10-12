#include "GameObject.h"
#include "Components.h"
#include "Component_Mesh.h"
GameObject::GameObject(std::string name)
{
	LOG("%s", name);
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
