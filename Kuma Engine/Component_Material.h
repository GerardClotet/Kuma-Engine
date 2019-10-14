#ifndef COMPONENT_MATERIAL_H
#define COMPONENT_MATERIAL_H

#include "Components.h"

struct TexData;

class Component_Material :public Components 
{
public:
	Component_Material();
	~Component_Material();


	bool Update();
	bool Enable();
	bool Disable();

public:
	TexData* texture = nullptr;
};


#endif // !COMPONENT_MATERIAL_H
