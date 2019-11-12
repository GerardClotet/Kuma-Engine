#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Application.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
#include <string>
class Components;
class Component_Material;
class Component_Mesh;
class Component_Transform;
class Component_Camera;
enum class GO_COMPONENT;
struct aiMesh;
struct aiNode;

struct meshInfo;

struct BoundingBox
{
	OBB obb;
	AABB aabb_local;
	AABB aabb_global;
	float3 min, max;
};
 enum class OBJECT_TYPE {
	PARENT,
	SUBPARENT,
	CUBE,
	SPHERE,
	CYLINDER,
	CONE,
	PLANE,
	IMPORTER,
	DODECAHEDRON,
	TORUS,
	NONE

};

class GameObject {


public:
	GameObject();
	GameObject(GameObject* parent,OBJECT_TYPE type,std::string name);
	/*GameObject(std::string name, OBJECT_TYPE type);
	GameObject(std::string name, OBJECT_TYPE type, aiMesh* mesh);*/
	~GameObject();
	bool Update();
	void Set_Parent_and_Name(GameObject* parent, std::string path);
	void RemoveGameObject(GameObject* child);
	void RemoveSubChildGameObject(GameObject* subchild);
	bool CleanUp();
	void CheckName(std::string name);
	bool hasComponent(GO_COMPONENT com);
	void SetBoundingBox();
	void TransformBBox();
	void DrawBoundingBox();
	void SaveToMeta(const char* path);
public:
	std::string name;
	std::string new_name;
	char c;
	double name_counter;
	char buf[4096];

	bool show		= false;

	OBJECT_TYPE type = OBJECT_TYPE::PARENT;
	float3 game_object_pos		= { 0.0f, 0.0f, 0.0f };
	float3 game_object_rot		= { 0.0f, 0.0f, 0.0f };
	float3 game_object_scale	= { 0.0f, 0.0f, 0.0f };

	Components* AddComponent(GO_COMPONENT type);
	Components* AddComponent(GO_COMPONENT type, aiMesh* mesh, aiNode* node);
	Components* AddComponent(GO_COMPONENT type, float3 pos, float3 scale, Quat rot);
	Components* AddComponent(GO_COMPONENT type, meshInfo* info);

	GameObject* parent				= nullptr; // to the scene invisible game object containing all gameobjects
	Component_Material* material	= nullptr;
	Component_Mesh* mesh			= nullptr;
	Component_Transform* transform	= nullptr;
	Component_Camera*camera			= nullptr;
	Components* component			= nullptr;

	std::vector<Components*> components;
	std::vector<GameObject*> game_object_childs;

	bool texture_Checker = false;

	BoundingBox bbox;
};



#endif // !GAMEOBJECT_H
