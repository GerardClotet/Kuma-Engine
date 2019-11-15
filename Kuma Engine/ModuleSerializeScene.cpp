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
		SaveScene("test");
	return UPDATE_CONTINUE;
}

void ModuleSerializeScene::LoadScene()
{


}

void ModuleSerializeScene::SaveScene(const char* name) 
{
	std::string path_to_save = name;

	path_to_save = LIBRARY_SCENE_FOLDER + path_to_save + EXTENSION_SCENE;
	
	App->fs->NormalizePath(path_to_save);

	/*const char* a = "C://Users//Gerard Clotet//Documents//GitHub//Kuma-Engine//Kuma Engine//Game//Library//Scenes//test.kumascene";
	const char* ap = App->fs->GetWritePath();
	App->fs->SplitFilePath(a->c_str(),a);*/
	//App->fs->NormalizePath(*a);
	R_JSON_File* scene = json_serializer->R_JSONWrite(path_to_save.c_str());

	if (scene != nullptr)
	{
		R_JSON_Value* go = scene->NewValue(rapidjson::kArrayType);
		GameObject* parent_root = App->scene_intro->root;

		std::vector<GameObject*>::iterator it = parent_root->game_object_childs.begin();

		while(it < parent_root->game_object_childs.end())
		{
			(*it)->SaveToScene(go);
			++it;
		}

		scene->AddValue("GameObject Root", *go);
		scene->WriteFile();
		
	}
	json_serializer->R_JSON_Close(scene);

}
