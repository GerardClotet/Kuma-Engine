
#include "Globals.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "par_shapes.h"
#include "ModuleSceneIntro.h"

#include "PanelConfig.h"
#include "ModuleUI.h"
#include "GameObject.h"
#include "Components.h"
#include "Component_Material.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")



ModuleImporter::ModuleImporter(Application* app, bool start_enabled) :Module(app, start_enabled)
{
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Init()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return true;
}

bool ModuleImporter::Start()
{
	//CreateCube({ 5,0,-5 }, { 0,0,0.0f});



	return true;
}

update_status ModuleImporter::Update(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleImporter::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}



bool ModuleImporter::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

void ModuleImporter::getImportedName(const char* path)
{
	

	LOG("paths %s",path);
	//Get the rout wihtout the name
	std::string file = path;
	size_t size_to_erase = file.rfind("\models");
	if (std::string::npos != size_to_erase)
		file.erase(size_to_erase);

	LOG("route %s", file.c_str());

	imported_route = file + "textures\\";

	//Get The name
	file= path;	
	std::string name = file.substr(file.find_last_of("\\") + 1);

	size_to_erase = name.rfind('.');

	if (std::string::npos != size_to_erase)
	{
		name.erase(size_to_erase);

	}
	LOG("name %s", name.c_str());

	imported_name = name;

	
}

void ModuleImporter::LoadImportedMaterials(std::string path)
{
	if (App->scene_intro->selected_game_obj != nullptr)
	{
		if (App->scene_intro->selected_game_obj->type == OBJECT_TYPE::SUBPARENT)
		{
			std::vector<GameObject*>::iterator it = App->scene_intro->selected_game_obj->game_object_childs.begin();
			while (it != App->scene_intro->selected_game_obj->game_object_childs.end())
			{
				if ((*it)->material == nullptr)
					(*it)->AddComponent(GO_COMPONENT::MATERIAL);

				(*it)->material->ReadTexture(path.c_str());
				++it;
			}
			return;
		}
		else if (App->scene_intro->selected_game_obj->mesh != nullptr)
		{
			if (App->scene_intro->selected_game_obj->material == nullptr)
				App->scene_intro->selected_game_obj->AddComponent(GO_COMPONENT::MATERIAL);

			App->scene_intro->selected_game_obj->material->ReadTexture(path.c_str());
			return;
		}
	}
}


void ModuleImporter::LoadGeometry(const char* path)
{
	//Load all the shapes of the FBX loaded file  
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		LOG("this imported scene has %d meshes", scene->mNumMeshes);
		if (scene->mNumMeshes > 1)
		{
			
			getImportedName(path);
			GameObject* go_subparent;
			go_subparent = App->scene_intro->CreateGameObject(nullptr, OBJECT_TYPE::SUBPARENT, imported_name + " parent");
			go_subparent->AddComponent(GO_COMPONENT::TRANSFORM, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f,0.0f });
			App->scene_intro->selected_game_obj = go_subparent;
			LoadNode(scene, scene->mRootNode, path,go_subparent);
		}

		else LoadSingleMesh(scene, path, scene->mRootNode);

		
		aiReleaseImport(scene);
	}
	else LOG("Error loading scene %s", path);
}


void ModuleImporter::LoadNode(const aiScene* importfile, aiNode* file_node, const char* name, GameObject* subparent)
{
	for (uint i = 0; i < file_node->mNumMeshes; i++)
	{
		aiString a = file_node->mName;
		std::string mesh_name = a.data;
		GameObject* go = nullptr;
		//getImportedName(name);

		go = App->scene_intro->CreateGameObject(subparent, OBJECT_TYPE::IMPORTER, mesh_name);
		aiMesh* mesh;
		mesh = importfile->mMeshes[file_node->mMeshes[i]];
		go->AddComponent(GO_COMPONENT::MESH, mesh, file_node);
		
		unsigned int numat = importfile->mNumMaterials;
		LOG("%i", a);
		if (importfile->HasMaterials())
		{
			//Texture
			for (uint m = 0; m < importfile->mNumMaterials; ++m) //no sense this here, just testing
			{
				aiMaterial* material = importfile->mMaterials[m];
				aiString texture_path;
				if (/*material->GetTextureCount(aiTextureType_DIFFUSE)>0 && */material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path) == aiReturn_SUCCESS)
				{
					LOG("%s", texture_path.C_Str());
					LoadTextureFromMaterial(imported_route + texture_path.data,go);

				}
				else LOG("%s texture aiRETURN_FAILURE");
			}
			
		}
			//for (uint m = 0; m < importfile->mNumMaterials; ++i) //no sense this here, just testing
		
	}
	for (uint i = 0; i < file_node->mNumChildren; i++)
	{
		LoadNode(importfile, file_node->mChildren[i], name,subparent);

	}

}

void ModuleImporter::LoadTextureFromMaterial(std::string path, GameObject* game_object)
{
	if (game_object->material == nullptr)
	{
		game_object->AddComponent(GO_COMPONENT::MATERIAL);
		LOG("path %s", path.c_str());
		game_object->material->ReadTexture(path.c_str());
	}
	else game_object->material->ReadTexture(path.c_str());
	
}

void ModuleImporter::LoadSingleMesh(const aiScene* importfile, const char* name, aiNode* node)
{
	GameObject* go = nullptr;
	getImportedName(name);

	go = App->scene_intro->CreateGameObject(go, OBJECT_TYPE::IMPORTER, imported_name);

	aiMesh* mesh;
	mesh = importfile->mMeshes[0];//it has only 1 mesh
	
	go->AddComponent(GO_COMPONENT::MESH, mesh, node);
	App->scene_intro->selected_game_obj = go;

}

bool ModuleImporter::LoadModelFile(const char * model_file)
{
	//If the fbx wasn't loaded before, call the LoadGeometry and save a .meta
	//If the fbx was previously loaded, read all meta data in this function.
	//This function loads the info of the file, saving it to a struct and calling the component constructor,
	//passing this structure to the constructor

	//meshInfo* mesh_info;
	//mesh_info->num_vertex = meta_num_vertex... Do you understand it right?
	//gameObject->AddCOmponent(MESH, mesh_info);

	// To get the file name do: Split with the full path, and then GetFileName with that string
	// std::string test = LIBRARY_MODEL_FOLDER + file_name + "_meta.kuma";
	// if(App->fs->Exists(test.c_str()));
	//TO CREATE A FILE USE App->fs->SaveUnique();

	return true;
}

bool ModuleImporter::LoadTextureFile(const char * texture_file)
{
	return true;
}



