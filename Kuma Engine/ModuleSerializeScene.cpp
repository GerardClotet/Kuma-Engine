#include "ModuleSerializeScene.h"
#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleFileSystem.h"
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
		
			LoadScene("ep");

	
	}
	return UPDATE_CONTINUE;
}

void ModuleSerializeScene::LoadScene(const char* path)
{

	std::string path_load = path;
	path_load = ASSETS_SCENE_FOLDER + path_load + EXTENSION_SCENE; 

	if (!App->fs->Exists(path_load.c_str()))
	{
		LOG("PATH %s doesn't exists or can be found", path_load.c_str());
		return;
	}
	R_JSON_File* scene = json_serializer->R_JSONRead(path_load.c_str()); //good load

	if (scene != nullptr)
	{
		LOG("");
		R_JSON_Value* go = scene->GetValue("Camera");

		int a = 1127477312;
		go->GetInt( "UUID",a);

	}
}

void ModuleSerializeScene::SaveScene(const char* name)
{
	std::string path_to_save = name;
	current_scene = name;
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