
#ifndef _COMPONENTS_H__
#define _COMPONENTS_H__

#include "SerializerJSON.h"
#include <string>

class GameObject;
class Component_Material;
struct TexData;


typedef unsigned int uint;

enum class GO_COMPONENT
{
	NONE,
	MESH,
	TRANSFORM,
	MATERIAL,
	CAMERA
};
class Components
{
public:
	Components();
	virtual ~Components();


	virtual bool Update();
	virtual bool Enable();
	virtual bool Disable();

	virtual void SaveToMeta(const char* path);
	virtual void SaveScene(R_JSON_Value* component)const;

	GameObject* GetGameObject();
public:
	GO_COMPONENT comp_type = GO_COMPONENT::NONE;
	//GameObject* my_GameObject = nullptr;
	/*TexData* texture = nullptr;*/
	bool enabled			= true;
	bool setTexture			= false;
	std::string name;
	GameObject* gameObject_Item = nullptr;
};
#endif // !_COMPONENTS_H__




