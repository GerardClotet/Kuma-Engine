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
	void SetDefaultTexture();
	void QuitDefautTexture();
	TexData* GetTexture()const;

	TexData* texture = nullptr;
	std::string GetTexturePath()const;

	void SaveScene(R_JSON_Value* material)const;
public:
	
	std::string file_path;
	uint width = 0;
	uint height = 0;

	bool isTextureEnable = true;


private:
	TexData* temp_tex_holder = nullptr; //fill this tex in the cases the other texture is active
	std::string current_texture_name;
};


#endif // !COMPONENT_MATERIAL_H
