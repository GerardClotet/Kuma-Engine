#include "Component_Mesh.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "GameObject.h"
#include "Application.h"


Component_Mesh::Component_Mesh() : Components()
{

}

Component_Mesh::~Component_Mesh()
{

}
bool Component_Mesh::Update()
{
	return true;
}

bool Component_Mesh::Enable()
{
	enabled = true;
	return enabled;
}
bool Component_Mesh::Disable()
{
	enabled = false;
	return enabled;
}
//#include "Globals.h"
//#include "Component_Mesh.h"
//#include "Application.h"
//
//Component_Mesh::Component_Mesh(std::string name) : Component(name)
//{
//	name = "com_mesh";
//}
//
//Component_Mesh::~Component_Mesh()
//{
//}
//
//void Component_Mesh::Update()
//{
//
//	int s=0;
//}
//
////bool Component_Mesh::Enable()
////{
////	return true;
////}
//
////bool Component_Mesh::Disable()
////{
////	return true;
////}
