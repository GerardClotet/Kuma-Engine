#include "Component_Material.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "GameObject.h"
#include "ModuleFileSystem.h"

Component_Material::Component_Material(GameObject* obj)
{
	name = "material";
	this->gameObject_Item = obj;
	comp_type = GO_COMPONENT::MATERIAL;
}

Component_Material::~Component_Material()
{
	if(gameObject_Item->parent->game_object_childs.size() <2)
		App->texture->RemoveTexture(texture);


	texture = nullptr;
}

bool Component_Material::Update()
{

	if (strcmp(this->gameObject_Item->name.c_str(), "City_building_034") == 0)
	{
		LOG("SI");
	}

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
	if (this->texture == nullptr)
	{
		LOG("tex null");
		return;
	}
	width = this->texture->width;
	height = this->texture->height;
	setTexture = true;

	current_texture_name = this->texture->name;
}

void Component_Material::SetDefaultTexture()
{
	LOG("set default");
	if(gameObject_Item->game_object_childs.size()> 0)
	{
		std::vector<GameObject*>::iterator it = gameObject_Item->game_object_childs.begin();
		while (it < gameObject_Item->game_object_childs.end())
		{
			if ((*it)->material == nullptr)
				(*it)->AddComponent(GO_COMPONENT::MATERIAL);

			(*it)->material->texture = App->texture->GetDefaultTex();
			(*it)->texture_Checker = true;
			(*it)->material->setTexture = true;
			++it;
		}
	}
	else this->texture = App->texture->GetDefaultTex();
	setTexture = true;
}

void Component_Material::QuitDefautTexture()
{
	LOG("Quit default");

	std::vector<TexData*>::iterator it = App->texture->textures_vec.begin();
	while (it < App->texture->textures_vec.end())
	{	

		if ((*it)->name == current_texture_name || gameObject_Item->type == OBJECT_TYPE::SUBPARENT)
		{
			this->texture = (*it);
			LOG("changed to imported texture %s", (*it)->name.c_str());
			if (!gameObject_Item->game_object_childs.empty())
			{
			
				std::vector<GameObject*>::iterator item = gameObject_Item->game_object_childs.begin();

				while (item < gameObject_Item->game_object_childs.end())
				{
					if ((*it)->name == (*item)->material->current_texture_name);
					{
						(*item)->texture_Checker = false;
						(*item)->material->texture = (*it);
						LOG("texture %s added to gameobject %s ", (*it)->name.c_str(), (*item)->material->current_texture_name.c_str());
					}
					++item;
				}
			}
		}
		++it;
	}
}

TexData * Component_Material::GetTexture()const
{
	if (this->texture != nullptr)
		return this->texture;
}


std::string Component_Material::GetTexturePath()const
{

	return file_path;
}

void Component_Material::SaveScene(R_JSON_Value* v)const
{
	TexData* temp = GetTexture();
	R_JSON_Value* material = v->NewValue(rapidjson::kObjectType);
	//material
	material->SetString("Component", "Material");

	std::string t_path = GetTexturePath();
	App->fs->NormalizePath(t_path);
	//material->SetString("TextureName", temp->name.c_str());
	material->SetString("Texture Path", t_path.c_str());
	//For now just this

	v->AddValue("Material", *material);
}
