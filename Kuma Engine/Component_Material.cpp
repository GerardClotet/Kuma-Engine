#include "Component_Material.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "Application.h"
#include "ModuleTexture.h"



Component_Material::Component_Material(GameObject* obj)
{
	name = "material";
	this->gameObject_Item = obj;
	comp_type = GO_COMPONENT::MATERIAL;
}

Component_Material::~Component_Material()
{
	App->texture->RemoveTexture(texture);
	texture = nullptr;
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
	file_path = file;
	this->texture = App->texture->LoadTexture(file_path.c_str());
	width = this->texture->width;
	height = this->texture->height;
	setTexture = true;
	current_texture_name = this->texture->name;
}

void Component_Material::SetDefaultTexture()
{

	this->texture = App->texture->GetDefaultTex();
	setTexture = true;
}

void Component_Material::QuitDefautTexture()
{
	std::vector<TexData*>::iterator it = App->texture->textures_vec.begin();
	while (it < App->texture->textures_vec.end())
	{	

		if ((*it)->name == current_texture_name)
		{
			this->texture = (*it);
			LOG("changed to imported texture %s", (*it)->name);
		}
		++it;
	}
}

TexData * Component_Material::GetTexture()
{
	if (this->texture != nullptr)
		return this->texture;
}

