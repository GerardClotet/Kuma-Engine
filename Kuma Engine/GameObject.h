#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Application.h"
#include "Globals.h"
#include "Components.h"

class GameObject {


public:
	GameObject(std::string name);

	Components* AddComponent(GO_COMPONENT type);
	~GameObject();


	GameObject* parent = nullptr; // to the scene invisible game object containing all gameobjects

	std::vector<Components*> components;
};



#endif // !GAMEOBJECT_H
