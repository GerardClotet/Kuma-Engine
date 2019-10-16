
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
	/*std::string a = file.substr(file.find_last_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789_-.Ññ/&")+1);
	LOG("asap rockly %s", a);*/
	std::string name = file.substr(file.find_last_of("\\") + 1);


	LOG("name %s", name.c_str());

	test = name.c_str();

	
}


void ModuleImporter::LoadGeometry(const char* path)
{
	//Load all the shapes of the FBX loaded file  
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		LoadNode(scene, scene->mRootNode,path);


		aiReleaseImport(scene);
	}
	else LOG("Error loading scene %s", path);
}

void ModuleImporter::LoadNode(const aiScene* importfile, aiNode* file_node,const char* name)
{
	for (uint i = 0; i < file_node->mNumMeshes; i++)
	{
		GameObject* go = nullptr;
		getImportedName(name);
		LOG("test %s", test);
		go = App->scene_intro->CreateGameObject(go, OBJECT_TYPE::IMPORTER,test);
		aiMesh* mesh;

		mesh = importfile->mMeshes[file_node->mMeshes[i]];
		go->AddComponent(GO_COMPONENT::MESH, mesh);
		go->AddComponent(GO_COMPONENT::MATERIAL);
		
	}
	for (uint i = 0; i < file_node->mNumChildren; i++)
	{
		LoadNode(importfile, file_node->mChildren[i],name);

	}

}


