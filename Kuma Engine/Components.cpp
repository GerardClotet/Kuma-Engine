
#include "Components.h"

Components::Components()
{

}

Components::~Components()
{

}
bool Components::Update()
{
	return true;
}
bool Components::Enable()
{
	return enabled = true;
}

bool Components::Disable()
{
	return enabled = false;
}

//#include "Globals.h"
//#include "Components.h"
//
//Component::Component()
//{
//}
//
//Component::Component(std::string name)
//{
//
//}
//
//Component::~Component()
//{
//}
//
//void Component::Update()
//{
//}

void Components::SaveToMeta(const char* path)
{

}

void Components::SaveScene(R_JSON_Value* component) const
{
}

GameObject * Components::GetGameObject()
{
	return gameObject_Item;
}
