#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "Components.h"
#include "Component_Mesh.h"
//#include "PanelConfig.h"
//#include "ModuleUI.h"
//#include "ModuleImporter.h"
GameObject::GameObject(std::string name, OBJECT_TYPE type)
{
	this->name = name;
	this->type = type;

	if (type == OBJECT_TYPE::CUBE)
	{
		//components.push_back(AddComponent(GO_COMPONENT::TRANSFORM));
		components.push_back(AddComponent(GO_COMPONENT::MESH));
	}
}



Components* GameObject::AddComponent(GO_COMPONENT type)
{
	Components* component = nullptr;
	switch (type)
	{
	case GO_COMPONENT::NONE:
		break;
	case GO_COMPONENT::MESH:
		component = new Component_Mesh(this->type);
		break;
	case GO_COMPONENT::TRANSFORM:
		break;
	case GO_COMPONENT::MATERIAL:
		break;
			
	}
	return component;
}

GameObject::~GameObject()
{
}

bool GameObject::Update()
{
	for (std::vector<Components*>::iterator item_comp = components.begin(); item_comp != components.end(); ++item_comp)
	{
		//if ((*item_comp)->comp_type == GO_COMPONENT::MESH)
		//{

		//	//if (App->ui->config_p->Getwireframe() && App->ui->config_p->GetFill())
		//	//{
		//	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//	//	glPolygonOffset(1.0f, 0.375f); //test
		//	//	glColor4fv((float*)& App->importer->wire_color);
		//	//	glLineWidth(1.0f);

		//	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//	//	glColor4fv((float*)& App->importer->fill_color);

		//	//	(*item_comp)->Update();


		//	//}
		//	//else if (App->ui->config_p->GetFill())
		//	//{
		//	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//	//	glColor4fv((float*)& App->importer->fill_color);
		//	//	(*item_comp)->Update();

		//	//}
		//	//if (App->ui->config_p->Getwireframe())
		//	//{
		//	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//	//	glPolygonOffset(1.0f, 0.375f); //test
		//	//	glColor4fv((float*)& App->importer->wire_color);
		//	//	glLineWidth(1.0f);
		//	//	(*item_comp)->Update();

		//	//}
		//	(*item_comp)->Update();


		//}

		
			(*item_comp)->Update();
		
	}
	return true;
}
