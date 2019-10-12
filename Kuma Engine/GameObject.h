#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Application.h"
#include "Globals.h"
#include <string>

 enum class OBJECT_TYPE {
	NONE,
	CUBE,
	SPHERE,
	CYLINDRE,
	CAPSULE,
	PLANE

};

class GameObject {

public:
	GameObject(std::string name, OBJECT_TYPE type);
	~GameObject();
	bool Update();

public:
	std::string name;
	OBJECT_TYPE type = OBJECT_TYPE::NONE;

};



#endif // !GAMEOBJECT_H
