#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Application.h"
#include "Globals.h"
#include <string>

class Components;
enum class GO_COMPONENT;

 enum class OBJECT_TYPE {
	NONE,
	CUBE,
	SPHERE,
	CYLINDRE,
	CAPSULE,
	PLANE,
	IMPORTER

};

class GameObject {


public:

	GameObject(std::string name, OBJECT_TYPE type);
	~GameObject();
	bool Update();

public:
	std::string name;
	OBJECT_TYPE type = OBJECT_TYPE::NONE;

	Components* AddComponent(GO_COMPONENT type);

	GameObject* parent = nullptr; // to the scene invisible game object containing all gameobjects

	std::vector<Components*> components;

};



#endif // !GAMEOBJECT_H
