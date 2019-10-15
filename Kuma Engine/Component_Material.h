#ifndef COMPONENT_MATERIAL_H
#define COMPONENT_MATERIAL_H

#include "Components.h"

struct TexData;

class GameObject;

class Component_Material :public Components 
{
public:
	Component_Material(GameObject* obj);
	~Component_Material();


	bool Update();
	bool Enable();
	bool Disable();
	void ReadTexture(std::string file);
	TexData* GetTexture();

	TexData* texture = nullptr;

public:
	GameObject* gameObject_Item = nullptr;
	std::string file_path;
};


#endif // !COMPONENT_MATERIAL_H
