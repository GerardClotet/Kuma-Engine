#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "Components.h"
#include "Component_Mesh.h"
#include "Component_Material.h"
#include "PanelConfig.h"
#include "ModuleUI.h"
#include "ModuleImporter.h"

GameObject::GameObject()
{
	LOG("Game Object root");
}

GameObject::GameObject(GameObject* parent,OBJECT_TYPE type,const char* nom)
{
	
	name = nom;
	this->type = type;
	Set_GO_Parent(parent);
	LOG("game object name %s", this->name);
}

//GameObject::GameObject(std::string name, OBJECT_TYPE type)
//{
//	this->name = name;
//	this->type = type;
//
//	//if (type == OBJECT_TYPE::CUBE || type == OBJECT_TYPE::SPHERE || type == OBJECT_TYPE::CONE || type == OBJECT_TYPE::CYLINDER || type ==  OBJECT_TYPE::DODECAHEDRON)
//	//{
//		//components.push_back(AddComponent(GO_COMPONENT::TRANSFORM));
//		components.push_back(AddComponent(GO_COMPONENT::MESH));
//	//}
//
//}
//
//GameObject::GameObject(std::string name, OBJECT_TYPE type, aiMesh * mesh)
//{
//	this->name = name;
//	this->type = type;
//
//	if (type == OBJECT_TYPE::IMPORTER)
//	{
//		//components.push_back(AddComponent(GO_COMPONENT::TRANSFORM));
//		components.push_back(AddComponent(GO_COMPONENT::MESH, mesh));
//	}
//
//}






Components* GameObject::AddComponent(GO_COMPONENT type)
{
	switch (type)
	{
	case GO_COMPONENT::NONE:
		break;
	case GO_COMPONENT::MESH:
		mesh = new Component_Mesh(this->type, this);
		components.push_back(mesh);

		break;
	case GO_COMPONENT::TRANSFORM:
		break;
	case GO_COMPONENT::MATERIAL:
		material = new Component_Material(this);
		components.push_back(material);
		break;
			
	}
	return component;
}

Components * GameObject::AddComponent(GO_COMPONENT type, aiMesh * mesh)
{
	this->mesh = new Component_Mesh(this->type, mesh,this);
	components.push_back(this->mesh);
	return component;
}

GameObject::~GameObject()
{
}

bool GameObject::Update()
{
	for (std::vector<Components*>::iterator item_comp = components.begin(); item_comp != components.end(); ++item_comp)
	{
		if ((*item_comp)->comp_type == GO_COMPONENT::MESH)
		{

			if (App->ui->config_p->Getwireframe() && App->ui->config_p->GetFill())
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glPolygonOffset(1.0f, 0.375f); //test
				glColor4fv((float*)& App->importer->wire_color);
				glLineWidth(1.0f);

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor4fv((float*)& App->importer->fill_color);

				(*item_comp)->Update();


			}
			else if (App->ui->config_p->GetFill())
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor4fv((float*)& App->importer->fill_color);
				(*item_comp)->Update();

			}
			if (App->ui->config_p->Getwireframe())
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glPolygonOffset(1.0f, 0.375f); //test
				glColor4fv((float*)& App->importer->wire_color);
				glLineWidth(1.0f);
				(*item_comp)->Update();

			}


		}

		
		else if ((*item_comp)->comp_type != GO_COMPONENT::MESH) { (*item_comp)->Update(); }
		
	}
	return true;
}

void GameObject::Set_GO_Parent(GameObject* go_parent)
{
	//if(parent != nullptr)
	if (parent == go_parent)
		return;


	parent = go_parent;
	go_parent->game_object_childs.push_back(this);
}

