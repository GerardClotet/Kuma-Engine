
#include "Globals.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "par_shapes.h"
#include "ModuleSceneIntro.h"
#include "Mesh.h"
#include "Cube.h"
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
	std::string file= path;
	LOG("filepath %s", file.c_str());
	
	std::string name = file.substr(file.find_last_of("\\") + 1);

	const size_t  extension = name.rfind('.');

	if (std::string::npos != extension)
	{
		name.erase(extension);

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
			go_subparent->AddComponent(GO_COMPONENT::TRANSFORM);
			App->scene_intro->selected_game_obj = go_subparent;
			LoadNode(scene, scene->mRootNode, path,go_subparent);
		}

		else LoadSingleMesh(scene, path);

		
		aiReleaseImport(scene);
	}
	else LOG("Error loading scene %s", path);
}


void ModuleImporter::LoadNode(const aiScene* importfile, aiNode* file_node, const char* name, GameObject* subparent)
{
	for (uint i = 0; i < file_node->mNumMeshes; i++)
	{
		aiString a = file_node->mName;
		std::string af = a.data;
		GameObject* go = nullptr;
		//getImportedName(name);

		go = App->scene_intro->CreateGameObject(subparent, OBJECT_TYPE::IMPORTER, af);
		aiMesh* mesh;

		mesh = importfile->mMeshes[file_node->mMeshes[i]];
		go->AddComponent(GO_COMPONENT::MESH, mesh);
		go->AddComponent(GO_COMPONENT::TRANSFORM);
	}
	for (uint i = 0; i < file_node->mNumChildren; i++)
	{
		LoadNode(importfile, file_node->mChildren[i], name,subparent);

	}

}


void ModuleImporter::LoadSingleMesh(const aiScene* importfile, const char* name)
{
	GameObject* go = nullptr;
	getImportedName(name);

	go = App->scene_intro->CreateGameObject(go, OBJECT_TYPE::IMPORTER, imported_name);

	aiMesh* mesh;
	mesh = importfile->mMeshes[0];//it has only 1 mesh
	
	go->AddComponent(GO_COMPONENT::MESH, mesh);
	App->scene_intro->selected_game_obj = go;

}



