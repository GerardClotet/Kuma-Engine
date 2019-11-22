#include "ModuleSerializeScene.h"
#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleFileSystem.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "Component_Transform.h"
#include "Component_Mesh.h"
#include "Component_Material.h"
#include "Component_Camera.h"
#include "ModuleImporter.h"
ModuleSerializeScene::ModuleSerializeScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	json_serializer = new SerializerR_JSON();
}

ModuleSerializeScene::~ModuleSerializeScene()
{
	delete json_serializer;
}

update_status ModuleSerializeScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		if (!current_scene.empty())
			SaveScene(current_scene.c_str());

		else {
			//display To set Name;
			//current_scene = name_to_get
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{

		LoadScene("test");


	}
	return UPDATE_CONTINUE;
}



void ModuleSerializeScene::parseRecursive(std::string scope
	, rapidjson::Value::ConstMemberIterator object
	/*, std::unordered_map<std::string, std::string>& values*/)
{
	if (scope.empty())
	{
		scope = object->name.GetString();
	}
	else
	{
		scope = scope + "::" + object->name.GetString();
	}
	auto inElement = scope + "::";

	rapidjson::Type r_type = object->value.GetType();

	if (object->value.IsObject())
	{
		for (auto it = object->value.MemberBegin(); it != object->value.MemberEnd(); ++it)
		{
			parseRecursive(scope, it/*, values*/);
		}
	}

	else if (object->value.IsFloat())
	{
		float f_temp = object->value.GetFloat();
		sc_obj->inf_to_f->ChooseWhatToFill(object->name.GetString(), &f_temp);
	}
	else if (object->value.IsInt())
	{
		LOG("int %i", object->value.GetInt());
		sc_obj->inf_to_f->ChooseWhatToFill(object->name.GetString(), (int*)object->value.GetInt()); //this value will be added to his properfield

		
	}
	else if (object->value.IsArray())
	{
		LOG("");

		for (rapidjson::SizeType i = 0; i < object->value.Size(); i++)
		{
			rapidjson::Type array_type;


			array_type = object->value[i].GetType();
			
			//const char* array_str = object->value[i].GetString();

			if (object->value[i].IsObject())
			{
				//std::unordered_map<std::string, std::string > _unorder;



				for (auto autom = object->value[i].MemberBegin(); autom != object->value[i].MemberEnd(); ++autom)
				{
					const char* child_name = (*autom).name.GetString();

					parseRecursive("", autom/*, _unorder*/);
				}
			}

			else if (object->value[i].IsNumber()) //must especify what kind of number to later properly cast
			{
				//object->name[i] doesnt exist because its an array

				if (object->value[i].IsFloat())  
				{
					float f_temp = object->value[i].GetFloat();
					sc_obj->inf_to_f->ChooseWhatToFill(object->name.GetString(), &f_temp);  
				}

				else if (object->value[i].IsInt())
					sc_obj->inf_to_f->ChooseWhatToFill(object->name.GetString(), (void*)object->value[i].GetInt());

				else if (object->value[i].IsUint())
					sc_obj->inf_to_f->ChooseWhatToFill(object->name.GetString(), (void*)object->value[i].GetUint());

				else if (object->value[i].IsUint64())
					sc_obj->inf_to_f->ChooseWhatToFill(object->name.GetString(), (void*)object->value[i].GetUint64());

				else if (object->value[i].IsInt64())
					sc_obj->inf_to_f->ChooseWhatToFill(object->name.GetString(), (void*)object->value[i].GetInt64());
			}
			else if (object->value[i].IsString())
				sc_obj->inf_to_f->ChooseWhatToFill(object->name.GetString(), (void*)object->value[i].GetString());

			else if (object->value[i].IsArray())
				LOG("Shouldnt");

			else
			{
				//LOG(" Selected_r_jsonType %i", selectR_jsonType(array_type));
			}

			//printf("a[%d] = %d\n", i, object->value[i].GetInt());
		}
		//values.emplace(inElement, (object->value.GetArray()));
	}
	else if (object->value.IsString())
	{
		
		sc_obj->inf_to_f->ChooseWhatToFill(object->name.GetString(), (void*)object->value.GetString());
		
	}
	else if (object->value.IsUint64())
	{
		sc_obj->inf_to_f->ChooseWhatToFill(object->name.GetString(), (void*)object->value.GetUint64());
		LOG("");
	}
	else if (object->value.IsInt64())
	{
		sc_obj->inf_to_f->ChooseWhatToFill(object->name.GetString(), (void*)object->value.GetInt64());
		LOG("");
	}
	else
	{
		LOGW("Unsuported: " << inElement << object->name.GetString());
	}
}


void ModuleSerializeScene::LoadScene(const char* path)
{

	std::string path_load;
	path_load = App->fs->GetFileName(path);
	path_load = ASSETS_SCENE_FOLDER + path_load + EXTENSION_SCENE; 

	if (!App->fs->Exists(path_load.c_str()))
	{
		LOG("PATH %s doesn't exists or can be found", path_load.c_str());
		return;
	}

	sc_obj = new SceneObjects;
	
	//std::unordered_map<std::string , std::string > _unorder;
	R_JSON_File* scene = json_serializer->R_JSONRead(path_load.c_str()); //good load
	if (!scene)
		return;

	auto aut = scene->document->MemberBegin(); //just 1 member (root)
	
		if ((*aut).value.IsArray())
		{
			for (rapidjson::SizeType i = 0; i < aut->value.Size(); i++) //number of gameObjects (parents) in the scene
			{
				//Here create GameObject
				sc_obj->inf_to_f = new infoToFill; 
				sc_obj->inf_to_f->go_to_fill = new GameObject(); 
				sc_obj->inf_to_f->go_to_fill->type = OBJECT_TYPE::IMPORTER;
				for (auto autom = aut->value[i].MemberBegin(); autom != aut->value[i].MemberEnd(); ++autom)
				{
					//here set gameobject var like UUID, here passes the component array
					const char* child_name = (*autom).name.GetString(); //make a pool compare to know what to assing the given value

					parseRecursive("", autom/*, _unorder*/);
				}

				sc_obj->scene_go_vec.push_back(sc_obj->inf_to_f->go_to_fill);
				delete sc_obj->inf_to_f;
			}
	
		}
		
	
		sc_obj->FamilyGameObjects(sc_obj->scene_go_vec); 


		sc_obj->CleanUp();
		//assure sc_obj pointers are deleted;

		delete sc_obj;


	json_serializer->R_JSON_Close(scene);

	

}

void ModuleSerializeScene::SaveScene(const char* name)
{
	std::string path_to_save;
	current_scene = name;
	path_to_save = App->fs->GetFileName(name);
	path_to_save = ASSETS_SCENE_FOLDER + path_to_save + EXTENSION_SCENE; //change to assets scene
	
	if (!App->fs->Exists(path_to_save.c_str()))
	{
		std::string output;
		char* buffer = new char[0];

		App->fs->SaveUnique(output, buffer, 0, path_to_save.c_str());

	}

	App->fs->NormalizePath(path_to_save);


	R_JSON_File* scene = json_serializer->R_JSONWrite(path_to_save.c_str());

	if (scene != nullptr)
	{
		R_JSON_Value* go = scene->NewValue(rapidjson::kArrayType);
		GameObject* parent_root = App->scene_intro->root;

		std::vector<GameObject*>::iterator it = parent_root->game_object_childs.begin();

		while (it < parent_root->game_object_childs.end())
		{
			(*it)->SaveToScene(go);
			++it;
		}

		scene->AddValue("GameObject Root", *go);
		scene->WriteFile();

	}
	json_serializer->R_JSON_Close(scene);

}



void SceneObjects::FamilyGameObjects(std::vector<GameObject*> go)
{

	std::vector<GameObject*> parent_or_single;
	std::vector<GameObject*> dependant;

	

	///___Emparent____
	for (auto node = go.begin(); node < go.end(); ++node) //divide into 2 different groups
	{
		
		if ((*node)->Parent_UUID == 1)
			parent_or_single.push_back((*node));

		else dependant.push_back(*node);

	}

	for (auto p_s = parent_or_single.begin(); p_s < parent_or_single.end(); ++p_s)
	{

		(*p_s)->Set_Parent(nullptr);

		
		for (auto d = dependant.begin(); d < dependant.end(); ++d)
		{
			if ((*d)->Parent_UUID == (*p_s)->UUID)
			{
				(*d)->Set_Parent((*p_s));
			}
		}

	}
	
	for (auto node = go.begin(); node < go.end(); ++node)
	{
		(*node)->CheckName((*node)->name);
	}

}

bool SceneObjects::CleanUp()
{


	 std::vector<GameObject*>::iterator it = scene_go_vec.begin();

	 while (it < scene_go_vec.end())
	 {
		 

		// delete it;
		 ++it;
	 }

	 scene_go_vec.clear();
	 return true;
}





void infoToFill::ChooseWhatToFill(const char* field,void* undefined)
{

	const char* c;
	int a;
	uint ua;
	float dest;
	uint64 u64;
	__int64 i64;




	Go_ElementToFill(field);

	switch (go_p)
	{
	case GO_Properties::UUID:
		go_to_fill->UUID = static_cast<uint64>(reinterpret_cast<uintptr_t>(undefined)); //void to uint64 & int64
		break;
	case GO_Properties::PARENT_UUID:
		go_to_fill->Parent_UUID = static_cast<uint64>(reinterpret_cast<uintptr_t>(undefined)); //void to uint64 & int64
		break;
	case GO_Properties::NAME:
		go_to_fill->name = static_cast<const char* const>(undefined); //void to const char*
		go_to_fill->staticName = static_cast<const char* const>(undefined);
		break;
	case GO_Properties::COMPONENT:

		LOG("component %s", field);
		 c = static_cast<const char* const>(undefined);
		//need to se what component is 
		 if (strcmp(c, "Transform") == 0) { LOG("Transform will be Added"); }
		 else if (strcmp(c, "Mesh") == 0) { LOG("Mesh will be Added"); }
		 else if (strcmp(c, "Camera") == 0) 
		 { 
			 go_to_fill->AddComponent(GO_COMPONENT::CAMERA, true); //SetUp Manually
			 //App->scene_intro->camera_list.push_back(go_to_fill->camera);
			 go_to_fill->camera->gameObject_Item = go_to_fill; //Link Go & Component
			 go_to_fill->camera->frustum.pos = go_to_fill->transform->GetGlobalPosition();
			 LOG("Component Camera  Added");
		 }
		 else if (strcmp(c, "Material") == 0)
		 {
			 go_to_fill->AddComponent(GO_COMPONENT::MATERIAL);
		 }
		 //El material tambe s'ha de carregar un cop es tingui el pathtexture
			//go_to_fill->AddComponent()
		break;
	case GO_Properties::POSITION:
		//count x y z to set proper
		memcpy(&dest, undefined, sizeof(float));
		if (_counter == 0)
			t_pos.x = dest;
		else if (_counter == 1)
			t_pos.y = dest;
		else if (_counter == 2)
			t_pos.z = dest;
		_counter++;
		if (_counter == 3)
			_counter = 0;
		break;
	case GO_Properties::ROTATION:
		memcpy(&dest, undefined, sizeof(float));
		if (_counter == 0)
			t_rot.x = dest;
		else if (_counter == 1)
			t_rot.y = dest;
		else if (_counter == 2)
			t_rot.z = dest;
		else if (_counter == 3)
			t_rot.w = dest;

		_counter++;
		if (_counter == 4)
			_counter = 0;
		break;
	case GO_Properties::SCALE:
		memcpy(&dest, undefined, sizeof(float));
		if (_counter == 0)
			t_scale.x = dest;
		else if (_counter == 1)
			t_scale.y = dest;
		else if (_counter == 2)
			t_scale.z = dest;
		_counter++;
		if (_counter == 3)
		{
			_counter = 0;
			go_to_fill->AddComponent(GO_COMPONENT::TRANSFORM, t_pos, t_scale, t_rot);
			go_to_fill->transform->RecalculateTransformMatrix();
			go_to_fill->SetBoundingBox();

		}

		break;
	case GO_Properties::META_ROUTE:
		c = static_cast<const char* const>(undefined);  
		
		go_to_fill->AddComponent(GO_COMPONENT::MESH,App->importer->LoadMeshFromMeta(c));
		//dsgo_to_fill->mesh->mesh_meta_route = static_cast<const char* const>(undefined);
		break;
	case GO_Properties::TEXTURE_PATH:
		 //Aqui
	
		if (go_to_fill->mesh != nullptr)
		{
			go_to_fill->mesh->path_texture_associated_meta = static_cast<const char* const>(undefined);
			if (go_to_fill->mesh->path_texture_associated_meta != nullptr)
				go_to_fill->material->ReadTexture(static_cast<const char* const>(undefined));
		}
		break;
	case GO_Properties::FRUSTRUM_TYPE:

		go_to_fill->camera->frustum.type = static_cast<FrustumType>(static_cast<int>(reinterpret_cast<intptr_t>(undefined)));
		break;
	case GO_Properties::ASPECT_RATIO:
		break;
	case GO_Properties::NEAR_PLANE:
		memcpy(&dest, undefined, sizeof(float));
		go_to_fill->camera->frustum.nearPlaneDistance = dest;
		go_to_fill->camera->near_plane = dest;
		break;
	case GO_Properties::FAR_PLANE:
		memcpy(&dest, undefined, sizeof(float));
		go_to_fill->camera->frustum.farPlaneDistance = dest;
		go_to_fill->camera->far_plane = dest;
		break;
	case GO_Properties::HORIZONTAL_FOV:

		memcpy(&dest, undefined, sizeof(float));
		go_to_fill->camera->frustum.horizontalFov = dest;
		go_to_fill->camera->horizontal_fov = dest;
		break;
	case GO_Properties::VERTICAL_FOV:
		memcpy(&dest, undefined, sizeof(float));
		go_to_fill->camera->frustum.verticalFov = dest;
		go_to_fill->camera->vertical_fov = dest;
		break;
	case GO_Properties::FRUSTRUM_COLOR: //doesnt modify alpha
		memcpy(&dest, undefined, sizeof(float));
		if (_counter == 0)
			go_to_fill->camera->color_frustum.r = dest;
		else if(_counter == 1)
			go_to_fill->camera->color_frustum.g = dest;
		else if (_counter == 2)
			go_to_fill->camera->color_frustum.b = dest;
		_counter++;
		if (_counter == 3)
			_counter = 0;
		break;
	case GO_Properties::CAMERA_BACKGROUND_COLOR://doesnt modify alpha

		memcpy(&dest, undefined, sizeof(float));
		if (_counter == 0)  
			go_to_fill->camera->color_camera_bg.r = dest;
		else if (_counter == 1)
			go_to_fill->camera->color_camera_bg.g = dest;
		else if (_counter == 2)
			go_to_fill->camera->color_camera_bg.b = dest;
		_counter++;
		if (_counter == 3)
			_counter = 0;
		break;
	case GO_Properties::FRONT:
		memcpy(&dest, undefined, sizeof(float));
		if (_counter == 0)
			go_to_fill->camera->frustum.front.x = dest;
		else if (_counter == 1)
			go_to_fill->camera->frustum.front.y = dest;
		else if (_counter == 2)
			go_to_fill->camera->frustum.front.z = dest;
		_counter++;
		if (_counter == 3)
			_counter = 0;
		break;

	case GO_Properties::UP:
		memcpy(&dest, undefined, sizeof(float));
		if (_counter == 0)
			go_to_fill->camera->frustum.front.x = dest;
		else if (_counter == 1)
			go_to_fill->camera->frustum.front.y = dest;
		else if (_counter == 2)
			go_to_fill->camera->frustum.front.z = dest;
		_counter++;
		if (_counter == 3)
		{	
			go_to_fill->camera->ReloadFrustrum(); //RealodFrustum; !!! Necessary to work
			_counter = 0;
		}
		break;

	case GO_Properties::TYPE:

		a = static_cast<int>(reinterpret_cast<intptr_t>(undefined));

		//if(go_to_fill->material!=nullptr/* || go_to_fill->IsParShape(a)*/)
		//	go_to_fill->mesh->SetType(a);

			if (go_to_fill->IsParShape(a))
			{
				go_to_fill->AddComponent(GO_COMPONENT::MESH);
				go_to_fill->mesh->SetType(a);
			}
		//static_cast<OBJECT_TYPE>(static_cast<int>(reinterpret_cast<intptr_t>(undefined)))
		LOG("Cannot find which property has to be filled");
		break;
	case GO_Properties::NONE:
		LOG("Cannot find which property has to be filled");
		break;


	default:
		break;
	}
	

}

void infoToFill::Go_ElementToFill(const char* field)
{

	for (int i = 0; i < 20; ++i) 
	{
	
		if (i == 17 || i == 18 || i == 19)
			LOG("name1 %s", names_r_json[17]);

		if (strcmp(field, names_r_json[i]) == 0)
		{
		
			LOG("fff %s", field);
			go_p = static_cast<GO_Properties>(i);
			return;
		}
		else go_p = GO_Properties::NONE;
	}
}
