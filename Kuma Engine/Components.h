
#ifndef _COMPONENTS_H__
#define _COMPONENTS_H__

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
	MATERIAL
};
class Components
{
public:
	Components();
	virtual ~Components();


	virtual bool Update();
	virtual bool Enable();
	virtual bool Disable();


public:
	GO_COMPONENT comp_type = GO_COMPONENT::NONE;
	GameObject* my_GameObject = nullptr;
	/*TexData* texture = nullptr;*/
	bool enabled = true;
	bool setTexture = false;
};
#endif // !_COMPONENTS_H__



//#ifndef _COMPONETS_H__
//#define _COMPONENTS_H__
//#include <string>
////enum COMP_TYPE
////{
////
////	NONE,
////	MESH,
////	TRANSFORM,
////	MATERIAL,
////
////}type;
//
//class Component
//{
//public:
//	Component();
//	Component(std::string name);
//	~Component();
//
//	//COMP_TYPE type = COMP_TYPE::NONE;
//	virtual void Update();
//	//virtual bool Enable() {};
//	//virtual bool Disable() {};
//
//};
//
//#endif
