#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Application.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include <string>
class Components;
class Component_Material;
class Component_Mesh;
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
	GameObject(GameObject* parent,OBJECT_TYPE type,const char* name);
	/*GameObject(std::string name, OBJECT_TYPE type);
	GameObject(std::string name, OBJECT_TYPE type, aiMesh* mesh);*/
	~GameObject();
	bool Update();
	void Set_Parent_and_Name(GameObject* parent, const char* path);
	void RemoveGameObject(GameObject* child);
	bool CleanUp();
	void CheckName(const char* name);
public:
	const char* name;
	std::string new_name;
	char c;
	double name_counter;
	char buf[4096];
	OBJECT_TYPE type = OBJECT_TYPE::NONE;
	float3 game_object_pos		= { 0.0f, 0.0f, 0.0f };
	float3 game_object_rot		= { 0.0f, 0.0f, 0.0f };
	float3 game_object_scale	= { 0.0f, 0.0f, 0.0f };

	Components* AddComponent(GO_COMPONENT type);
	Components* AddComponent(GO_COMPONENT type, aiMesh* mesh);

	GameObject* parent				= nullptr; // to the scene invisible game object containing all gameobjects
	Component_Material* material	= nullptr;
	Component_Mesh* mesh			= nullptr;
	Components* component			= nullptr;

	std::vector<Components*> components;
	std::vector<GameObject*> game_object_childs;
};



#endif // !GAMEOBJECT_H
