#ifndef COMPONENT_MATERIAL_H
#define COMPONENT_MATERIAL_H

#include "Components.h"



class Component_Material :public Components 
{
public:
	Component_Material();
	~Component_Material();


	bool Update();
	bool Enable();
	bool Disable();

	
};


#endif // !COMPONENT_MATERIAL_H
