#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Application.h"
#include "Globals.h"
#include <string>

class Components;
enum class GO_COMPONENT;
struct aiMesh;

 enum class OBJECT_TYPE {
	NONE,
	CUBE,
	SPHERE,
	CYLINDER,
	CONE,
	PLANE,
	IMPORTER,
	DODECAHEDRON,
	TORUS

};

class GameObject {


public:
	GameObject();
	GameObject(GameObject* parent,OBJECT_TYPE type,std::string name);
	/*GameObject(std::string name, OBJECT_TYPE type);
	GameObject(std::string name, OBJECT_TYPE type, aiMesh* mesh);*/
	~GameObject();
	bool Update();
	void Set_GO_Parent(GameObject* parent);
public:
	std::string name;
	OBJECT_TYPE type = OBJECT_TYPE::NONE;

	Components* AddComponent(GO_COMPONENT type);
	Components* AddComponent(GO_COMPONENT type, aiMesh* mesh);

	GameObject* parent = nullptr; // to the scene invisible game object containing all gameobjects

	std::vector<Components*> components;
	std::vector<GameObject*> game_object_childs;
};



#endif // !GAMEOBJECT_H
