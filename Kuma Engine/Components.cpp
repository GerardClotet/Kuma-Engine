
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
void Components::ReadTexture(std::string file)
{
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
