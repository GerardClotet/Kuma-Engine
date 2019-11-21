#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "ModuleCamera3D.h"
#include "Components.h"
#include "Component_Mesh.h"
#include "Component_Material.h"
#include "Component_Transform.h"
#include "Component_Camera.h"
#include "PanelConfig.h"
#include "ModuleUI.h"
#include "ModuleImporter.h"
#include <string_view>
#include "RandomHelper.h"
GameObject::GameObject()
{
	//thought for constructing through SerializedScene and fill it later

}

GameObject::GameObject(const char* name) //just for root
{
	LOG("Game Object root");
	UUID = 1;

	Set_Parent_and_Name(nullptr, "Root");
}
GameObject::GameObject(GameObject* parent,OBJECT_TYPE type, std::string name)
{
	this->type = type;
	this->name = name;
	Set_Parent_and_Name(parent,name);
	//TODO :/Generate random and save it to ID
	UUID = GetRandomID();
	LOG("game object name %s", this->name.c_str());
	staticName = name;

}



Components* GameObject::AddComponent(GO_COMPONENT type,bool serialized_cam)
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
			
	case GO_COMPONENT::CAMERA:

		serialized_cam ?  camera = new Component_Camera() : camera = new Component_Camera(this);
		components.push_back(camera);
		component = camera;
		if (this->name != "Camera Fake")
		{
			App->scene_intro->camera_list.push_back(camera);
			if (App->scene_intro->selected_camera_obj == nullptr)
				App->scene_intro->selected_camera_obj = this;
		}


		break;
	}
	return component;
}

Components* GameObject::AddComponent(GO_COMPONENT type, aiMesh* mesh, aiNode* node)
{
	this->mesh = new Component_Mesh(this->type, mesh, this, node);
	components.push_back(this->mesh);
	return component;
}

Components* GameObject::AddComponent(GO_COMPONENT type, float3 pos, float3 scale, Quat rot)
{
	this->transform = new Component_Transform(this, pos, scale, rot);
	components.push_back(this->transform);
	return component;
}

Components* GameObject::AddComponent(GO_COMPONENT type, meshInfo* info)
{
	this->mesh = new Component_Mesh(this->type,info,this);
	components.push_back(this->mesh);
	return component;
}




GameObject::~GameObject()
{
//	LOG("deleted %s",name);
	

}

bool GameObject::Update()
{
	isInsideFrustum = CheckAABBinFrustum();
	if (this->hasComponent(GO_COMPONENT::TRANSFORM) && this->transform->boundingBoxActive)
		DrawBoundingBox();

	for (std::vector<Components*>::iterator item_comp = components.begin(); item_comp != components.end(); ++item_comp)
	{
		
		if ((*item_comp)->comp_type == GO_COMPONENT::MESH && isInsideFrustum)
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

		
		else if ((*item_comp)->comp_type != GO_COMPONENT::MESH) {
			(*item_comp)->Update(); 
		}
		
	}

	if (type == OBJECT_TYPE::SUBPARENT)
	{
		//TransformParentBBox(); //peta quan delete
		if (this->transform->boundingBoxActive)
			DrawBoundingBox();
		
	}
	return true;
}

void GameObject::Set_Parent_and_Name(GameObject* go_parent, std::string path)
{
	//if(parent != nullptr)
	if (parent == go_parent)
	{
		this->name = path;
		return;
	}

	parent = go_parent;
	go_parent->game_object_childs.push_back(this);

	CheckName(path);
}

void GameObject::Set_Parent(GameObject* go_parent)
{
	LOG("name %s", name.c_str());
	if (parent == go_parent)
	{
		parent = App->scene_intro->root;
		parent->game_object_childs.push_back(this);	
		return;
	}

	parent = go_parent;
	go_parent->game_object_childs.push_back(this);
}

void GameObject::RemoveGameObject(GameObject* child)
{
	std::vector<GameObject*>::iterator iter = App->scene_intro->root->game_object_childs.begin();

	while (iter != App->scene_intro->root->game_object_childs.end())
	{
		
		if ((*iter) == child)
		{
			(*iter)->CleanUp();
			LOG("deleted component");
			delete (*iter);
			App->scene_intro->root->game_object_childs.erase(iter);
			App->scene_intro->selected_game_obj = nullptr;
			//App->ui->inspector_window = false;
			return;
		}
		++iter;
	}
	//game_object_childs.(child);
}

void GameObject::RemoveSubChildGameObject(GameObject* subchild)
{

	std::vector<GameObject*>::iterator iter = App->scene_intro->root->game_object_childs.begin();

	while (iter != App->scene_intro->root->game_object_childs.end())
	{
		if ((*iter)->type == OBJECT_TYPE::SUBPARENT)
		{
			std::vector<GameObject*>::iterator it = (*iter)->game_object_childs.begin();

			while (it != (*iter)->game_object_childs.end())
			{

				if ((*it) == subchild)
				{
					(*it)->CleanUp();
					LOG("deleted component");
					delete (*it);
					(*iter)->game_object_childs.erase(it);
					App->scene_intro->selected_game_obj = nullptr;
					//App->ui->inspector_window = false;
					return;
				}
				++it;
			}
		}

		++iter;
	}
}

bool GameObject::CleanUp()
{
	LOG("---Cleaning %s---", name.c_str());
	
	//name = nullptr;
	std::vector<GameObject*>::iterator it = game_object_childs.begin();
	while (it != game_object_childs.end())
	{
		std::vector<Components*>::iterator item = (*it)->components.begin();
		while (item != (*it)->components.end())
		{
			LOG("deleted compoennt %s", (*item)->name.c_str()); //tindre en compte borrar la textura des del el parent
			delete (*item);
			++item;
		}
		(*it)->components.clear();

		delete (*it);
		++it;
	}
	game_object_childs.clear();
	LOG("gameobject childs deleted");

	std::vector<Components*>::iterator item = components.begin();
	while (item != components.end())
	{
		LOG("deleted compoennt %s", (*item)->name.c_str());
		delete (*item);
		++item;
	}
	components.clear();
	
	parent = nullptr;



	
	return true;
}

void GameObject::CheckName(std::string path)
{
	name_counter = 0;
	std::string temp_name = path;
	bool no_name = true;
	std::vector<GameObject*>::const_iterator iter = parent->game_object_childs.begin();
	while (iter != parent->game_object_childs.end())
	{
		if (strcmp((*iter)->name.c_str(), temp_name.c_str())==0)
		{
			if ((*iter) == this)
				return;
			

			
				no_name = false;
				++name_counter;
				std::string a = path;

				a = a + "(" + std::to_string((uint)name_counter) + ")";
				new_name = a;
				temp_name = new_name.c_str();
				this->name = new_name.c_str();
			
		}


		++iter;

	}



	if (no_name)
		this->name = path;
}

bool GameObject::hasComponent(GO_COMPONENT com)
{
	switch (com)
	{
	case GO_COMPONENT::NONE:
		break;

	case GO_COMPONENT::MESH:
		if (this->mesh != nullptr)
			return true;
		else
			return false;
		break;

	case GO_COMPONENT::TRANSFORM:
		if (this->transform != nullptr)
			return true;
		else
			return false;
		break;

	case GO_COMPONENT::MATERIAL:
		if (this->material != nullptr)
			return true;
		else
			return false;
		break;

	case GO_COMPONENT::CAMERA:
		if (this->camera != nullptr)
			return true;
		else
			return false;
		break;

	default:
		break;
	}
	
}

void GameObject::SetBoundingBox()
{
	if (mesh != nullptr)
	{
		bbox.aabb_local.SetNegativeInfinity(); //Set value to 0 /null
		bbox.aabb_local.Enclose((float3*)mesh->vertex, mesh->num_vertex);

		bbox.min = bbox.aabb_local.minPoint;
		bbox.max = bbox.aabb_local.maxPoint;

		TransformBBox();
	}
	
	
}

void GameObject::TransformBBox()
{
	bbox.obb = bbox.aabb_local;
	bbox.obb.Transform(transform->GetGlobalMatrix());

	bbox.aabb_global.SetNegativeInfinity();
	bbox.aabb_global.Enclose(bbox.obb);

	bbox.min = bbox.aabb_local.minPoint;
	bbox.max = bbox.aabb_local.maxPoint;

}

void GameObject::TransformParentBBox()
{

	//bbox.obb = bbox.aabb_local;
	//bbox.obb.Transform(transform->GetParentGlobalMatrix());

	bbox.aabb_global.SetNegativeInfinity();


	std::vector<GameObject*>::iterator it = game_object_childs.begin();


	while (it < game_object_childs.end())
	{
		bbox.obb = (*it)->GetAABB();
		bbox.obb.Transform((*it)->transform->GetGlobalMatrix());
		bbox.aabb_global.Enclose(bbox.obb);
		++it;
	}

}
AABB GameObject::GetAABB()
{
	if (mesh != nullptr)
	{
		AABB aabb;
		aabb.SetNegativeInfinity();
		aabb.Enclose((float3*)mesh->vertex, mesh->num_vertex);
		return aabb;
	}

	
}

void GameObject::DrawBoundingBox()
{
	for (int i = 0; i < bbox.aabb_global.NumEdges(); i++)
	{
		glBegin(GL_LINES);
		glLineWidth(1.0f);
		glColor3f(color_aabb.r, color_aabb.g, color_aabb.b);
		glVertex3f(bbox.aabb_global.Edge(i).a.x, bbox.aabb_global.Edge(i).a.y, bbox.aabb_global.Edge(i).a.z);
		glVertex3f(bbox.aabb_global.Edge(i).b.x, bbox.aabb_global.Edge(i).b.y, bbox.aabb_global.Edge(i).b.z);
		
		glEnd();

		glBegin(GL_LINES);
		glLineWidth(1.0f);
		glColor3f(color_obb.r, color_obb.g, color_obb.b);
		glVertex3f(bbox.obb.Edge(i).a.x, bbox.obb.Edge(i).a.y, bbox.obb.Edge(i).a.z);
		glVertex3f(bbox.obb.Edge(i).b.x, bbox.obb.Edge(i).b.y, bbox.obb.Edge(i).b.z);
		
		glEnd();
	}
}

void GameObject::GenerateParentBBox()
{


	std::vector<GameObject*>::iterator it = game_object_childs.begin();

	bbox.aabb_local.SetNegativeInfinity();


	while (it < game_object_childs.end())
	{
		
		bbox.aabb_local.Enclose((*it)->bbox.aabb_local);
		
		++it;
	}

	

	bbox.obb = bbox.aabb_local;
	
	//bbox.obb.Transform(transform->GetParentGlobalMatrix()); //dont needit

	bbox.aabb_global.SetNegativeInfinity();
	bbox.aabb_global.Enclose(bbox.obb);

	//bbox.aabb_local.Enclose()
}

bool GameObject::CheckAABBinFrustum()
{
	bool ret = true;
	if (App->camera->camera_fake->frustum.Intersects(this->bbox.aabb_global))
		ret = true;
	else
		ret = false;

	
		
	if (App->scene_intro->selected_camera_obj!=nullptr && App->scene_intro->selected_camera_obj->hasComponent(GO_COMPONENT::CAMERA))
	{
		if (App->camera->camera_fake->frustum.Intersects(App->scene_intro->selected_camera_obj->camera->frustum))
		{
			if (App->scene_intro->selected_camera_obj->camera->culling)
			{
				if (App->scene_intro->selected_camera_obj->camera->frustum.Intersects(this->bbox.aabb_global))
					ret = true;
				else
					ret = false;
			}
		}
	}
		

	

	return ret;
}

void GameObject::SaveToMeta(const char* path)//for now we just save mesh & texture not components
{
	std::vector<Components*>::iterator it = components.begin();
		

	while (it < components.end())
	{

		if ((*it) != nullptr)
		{
			(*it)->SaveToMeta(path);
		}
		++it;
	}
}


void GameObject::SaveToScene(R_JSON_Value* json_val)
{

	R_JSON_Value* go = json_val->NewValue(rapidjson::kObjectType);

	go->SetUint32("UUID", UUID);
	if (parent != nullptr)
		go->SetUint32("Parent UUID", parent->UUID);

	go->SetString("Name", name.c_str());

	R_JSON_Value* c = go->NewValue(rapidjson::kArrayType);

	std::vector<Components*>::iterator it = components.begin();

	while (it < components.end())
	{
		(*it)->SaveScene(c);
		++it;
	}

	go->AddValue("Components", *c);
	json_val->AddValue("GameObject", *go);

	for (auto& child : game_object_childs)
	{
		child->SaveToScene(json_val);
	}
}

void GameObject::RemoveCameraFromist(GameObject *obj)
{
	
	for (std::vector<Components*>::iterator item = App->scene_intro->camera_list.begin(); item != App->scene_intro->camera_list.end(); ++item)
	{
		if ((*item) == obj->camera)
		{
			for (std::vector<Components*>::iterator it = obj->components.begin(); it != obj->components.end(); ++it)
			{
				if ((*it) == (*item))
				{
					obj->components.erase(it);
					break;
				}
			}
			LOG("deleted compoennt %s", (*item)->name.c_str());
			delete (*item);

			//App->scene_intro->camera_list.erase(item);
			break;
			
			
		}
		LOG("HH");
	}

}

void GameObject::SetUUID(uint32 ID)
{
	UUID = ID;
}

bool GameObject::IsParShape(int obj_type)
{
	if (obj_type >= 2 && obj_type <= 6)
		return true;

	else if (obj_type > 8 && obj_type <= 9)
		return true;


	return false;
}
