#include "Component_Material.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "Application.h"
#include "ModuleTexture.h"



Component_Material::Component_Material(GameObject* obj)
{
	this->gameObject_Item = obj;
	comp_type = GO_COMPONENT::MATERIAL;
}

Component_Material::~Component_Material()
{
}

bool Component_Material::Update()
{
	return true;
}

bool Component_Material::Enable()
{
	return true;
}

bool Component_Material::Disable()
{
	return true;
}

void Component_Material::ReadTexture(std::string file)
{
	this->texture = App->texture->LoadTexture(file.c_str());
	setTexture = true;
}

TexData * Component_Material::GetTexture()
{
	if (this->texture != nullptr)
		return this->texture;
}
