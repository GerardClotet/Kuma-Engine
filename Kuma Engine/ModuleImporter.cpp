
#include "Globals.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "par_shapes.h"
#include "ModuleFileSystem.h"
#include "ModuleSceneIntro.h"

#include "PanelConfig.h"
#include "ModuleUI.h"
#include "GameObject.h"
#include "Components.h"
#include "Component_Material.h"
#include "Component_Mesh.h"
#include "DevIL/include/IL/ilu.h"
#include "DevIL/include/IL/ilut.h"
#include "DevIL/include/IL/il.h"


#pragma comment (lib, "Assimp/libx86/assimp.lib")

#pragma comment(lib,"Devil/libx86/DevIL.lib")
#pragma comment(lib,"Devil/libx86/ILU.lib")
#pragma comment(lib,"Devil/libx86/ILUT.lib")



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

	////Get The name
	//file= path;	
	//std::string name = file.substr(file.find_last_of("\\") + 1);

	//size_to_erase = name.rfind('.');

	//if (std::string::npos != size_to_erase)
	//{
	//	name.erase(size_to_erase);

	//}
	//LOG("name %s", name.c_str());
	std::string name;
	App->fs->SplitFilePath(path, nullptr, &name);
	name = App->fs->GetFileName(name.c_str());
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

				SaveTextureToMeta(path.c_str());
				++it;
			}
			return;
		}
		else if (App->scene_intro->selected_game_obj->mesh != nullptr)
		{
			if (App->scene_intro->selected_game_obj->material == nullptr)
				App->scene_intro->selected_game_obj->AddComponent(GO_COMPONENT::MATERIAL);

			App->scene_intro->selected_game_obj->material->ReadTexture(path.c_str());
			SaveTextureToMeta(path.c_str());
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
			
			model_info = new modelInfo;
			/*meshInfo* meshinf = new meshInfo;
			meshinf->name = "subparent";
			SaveMeshToMeta(path, meshinf);*/
			App->scene_intro->selected_game_obj = go_subparent;
			LoadNode(scene, scene->mRootNode, path,go_subparent);
			SaveModelToMeta(path, model_info);
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
		
		SaveMeshToMeta(name, go->mesh->saveMeshinfo()); //save it


		unsigned int numat = importfile->mNumMaterials;
		LOG("%i", a);
		if (importfile->HasMaterials())
		{
			//Texture
			for (uint m = 0; m < importfile->mNumMaterials; ++m) //no sense this here, just testing
			{
				aiMaterial* material = importfile->mMaterials[m];
				aiString texture_path;
				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path) == aiReturn_SUCCESS)
				{
					LOG("%s", texture_path.C_Str());
					LoadTextureFromMaterial(imported_route + texture_path.data,go);
					SaveTextureToMeta(texture_path.C_Str());

				}
				else LOG("%s texture aiRETURN_FAILURE");
			}
			
		}
		
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

std::string ModuleImporter::LoadModelConfig(JSON_Object*& config, const char* line)
{

	std::string temp = json_object_dotget_string(config,"line");



	return temp;
}

void ModuleImporter::SetString(JSON_Object*& file, const char* container )
{
	json_object_dotset_string(file, "Mesh.Route", container);

}

void ModuleImporter::SerializeJSONFile(const char* path,JSON_Value* save_value)
{
	json_serialize_to_file_pretty(save_value, path);
}

JSON_Object* ModuleImporter::LoadJSONFile(const std::string& path, JSON_Value* val)
{
	json_parse_file(path.data());
	JSON_Object* object = json_value_get_object(val);



	return object;
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

	unsigned int numat = importfile->mNumMaterials;

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
				LoadTextureFromMaterial(imported_route + texture_path.data, go);

			}
			else LOG("%s texture aiRETURN_FAILURE");
		}

	}
	model_info = new modelInfo;
	SaveMeshToMeta(name, go->mesh->saveMeshinfo()); //save it
	SaveModelToMeta(name, model_info);

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
	std::string path_meta = App->fs->GetModelMetaPath(model_file);
	if (App->fs->Exists(path_meta.c_str())) //encara que hagi estat carregat diu que no existeix pq el subparent no l'ha fet i aqui busca el meta del subparent;
	{
		//It was previously loaded
		//Read from the meta
		LoadModelFromMeta(model_file, path_meta.c_str());

	}
	else
	{
		//It wasn't loaded before
		LoadGeometry(model_file);
	}

	return true;
}

bool ModuleImporter::LoadTextureFile(const char * texture_file)
{
	std::string path_meta = App->fs->GetTextureMetaPath(texture_file);
	if (App->fs->Exists(path_meta.c_str()))
	{
		//It was previously loaded
		//Read from the meta
		/*meshInfo* info = LoadtoMeta(path_meta.c_str());
		GameObject* go = nullptr;
		getImportedName(model_file);

		go = App->scene_intro->CreateGameObject(nullptr, OBJECT_TYPE::IMPORTER, imported_name);

		go->AddComponent(GO_COMPONENT::MESH, info);
		go->AddComponent(GO_COMPONENT::TRANSFORM, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f,0.0f });*/

	}
	else
	{
		//It wasn't loaded before
		LoadImportedMaterials(std::string(texture_file));
	}
	return true;
}

void ModuleImporter::SaveMeshToMeta(const char* path,meshInfo* mesh)
{

	uint ranges[5] = {
		mesh->num_vertex,
		mesh->num_index ,
		mesh->num_normal,
		mesh->num_uvs,
		mesh->num_color
	};

	uint size = sizeof(ranges)
		+ sizeof(uint) * mesh->num_vertex * 3
		+ sizeof(uint) * mesh->num_index 
		+ sizeof(uint) * mesh->num_normal * 3
		+ sizeof(uint) * mesh->num_uvs * 2
		+ sizeof(uint) * mesh->num_color * 4;



	char* data = new char[size]; //Allocate
	char* cursor = data;


	uint bytes = sizeof(ranges); //Store ranges
	memcpy(cursor, ranges, bytes);


	cursor += bytes;
	bytes = sizeof(float) * mesh->num_vertex * 3;
	memcpy(cursor, mesh->vertex, bytes);

	cursor += bytes;//Store index;
	bytes = sizeof(uint) * mesh->num_index;
	memcpy(cursor, mesh->index, bytes);

	//----
	cursor += bytes;
	bytes = sizeof(float) * mesh->num_normal * 3;
	memcpy(cursor, mesh->normal, bytes);



	//---

	cursor += bytes;
	bytes = sizeof(float) * mesh->num_uvs * 2;
	memcpy(cursor, mesh->uvs, bytes);

	//--


	//---

	//----
	cursor += bytes;
	bytes = sizeof(float) * mesh->num_color * 4;
	memcpy(cursor, mesh->color, bytes);


	std::string name;
	if (mesh->name !="subparent") {
		 name = LIBRARY_MESH_FOLDER + mesh->name + EXTENSION_META;
		LOG("tets %s", name.c_str());

	}
	else
	{
		std::string temp = App->fs->GetFileName(path);
		name = LIBRARY_MESH_FOLDER + temp + EXTENSION_META;
	}

	std::string output;
	App->fs->SaveUnique(output, data, size, name.c_str());
	LOG("output %s 1 %s", output, output.c_str());
	mesh->route = name; //route to meta file

	model_info->meshinfo.push_back(mesh);
}

void ModuleImporter::LoadModelFromMeta(const char* original_path, const char* path)
{
	
	char* buffer = nullptr;
	uint testu = App->fs->Load(path, &buffer);

	char* cursor = buffer;
	uint num_meshes = 0;
	memcpy(&num_meshes, cursor, sizeof(uint));
	cursor += sizeof(uint);
	for (int i = 0; i < num_meshes; ++i)
	{
		uint size_str = 0;
		memcpy(&size_str, cursor, sizeof(uint));
		cursor += sizeof(uint);
		char* path_temp = new char [size_str];
		memcpy(path_temp, cursor, sizeof(char)*size_str);
		
		//tallar la string pq es passa de llarg
	}
	
}

void ModuleImporter::SaveTextureToMeta(const char * path)
{
	
	ILuint size;
	ILubyte *data;
	ilSetInteger(IL_DXTC_DATA_FORMAT, IL_DXT5);
	size = ilSaveL(IL_DDS, NULL, 0);
	if (size > 0)
	{
		data = new ILubyte[size];
		if (ilSaveL(IL_DDS, data, size) > 0)
		{
			std::string temp = App->fs->GetFileName(path);
			std::string test = LIBRARY_TEXTURES_FOLDER + temp + EXTENSION_TEXTURE_META;
			LOG("tets %s", test.c_str());
			std::string output;
			App->fs->SaveUnique(output, data, size, test.c_str());
		}
			
	}
}

meshInfo* ModuleImporter::LoadMeshtoMeta(const char* path)
{
		meshInfo* mesh = new meshInfo;

		uint ranges[5] = {
			mesh->num_vertex,
			mesh->num_index,
			mesh->num_normal,
			mesh->num_uvs,
			mesh->num_color
		};
		//Save the variables of meshInfo to the local variables. This is GG compared with the other ;)
		char* buffer;
		uint testu = App->fs->Load(path, &buffer);

		char* cursor = buffer;
		uint bytes = sizeof(ranges);

		memcpy(ranges, cursor, bytes);
		mesh->num_vertex = ranges[0];
		mesh->num_index = ranges[1];
		mesh->num_normal = ranges[2];
		mesh->num_uvs = ranges[3];
		mesh->num_color = ranges[4];

		cursor += bytes;
		bytes = sizeof(float) * mesh->num_vertex * 3;
		mesh->vertex = new float[mesh->num_vertex * 3];
		memcpy(mesh->vertex, cursor, bytes);

		cursor += bytes;
		bytes = sizeof(uint) * mesh->num_index;
		mesh->index = new uint[mesh->num_index];
		memcpy(mesh->index, cursor, bytes);

		cursor += bytes;
		bytes = sizeof(float) * mesh->num_normal * 3;
		mesh->normal = new float[mesh->num_normal * 3];
		memcpy(mesh->normal, cursor, bytes);


		cursor += bytes;
		bytes = sizeof(float) * mesh->num_uvs * 2;
		mesh->uvs = new float[mesh->num_uvs * 2];
		memcpy(mesh->uvs, cursor, bytes);



		cursor += bytes;
		bytes = sizeof(float) * mesh->num_color * 4;
		mesh->color = new float[mesh->num_color * 4];
		memcpy(mesh->color, cursor, bytes);

		return mesh;
}

void ModuleImporter::SaveModelToMeta(const char* path,modelInfo* model)
{
	std::string output;
	std::string temp = App->fs->GetFileName(path);
	std::string meta_path = LIBRARY_MODEL_FOLDER + temp + "_meta.json";
	uint size_path = 0;
	for (int i = 0; i < model->meshinfo.size(); ++i)
	{
		size_path += sizeof(char) * model->meshinfo[i]->route.size();

	}
	uint size = sizeof(uint) * model->meshinfo.size() + size_path + sizeof(uint);
	char* data = new char[size];
	char* cursor = data;

	uint num_meshes = model->meshinfo.size();
	memcpy(cursor, &num_meshes, sizeof(uint));
	cursor += sizeof(uint);

	for (int i = 0; i < model->meshinfo.size(); ++i)
	{
		uint temp = model->meshinfo[i]->route.size();
		memcpy(cursor, &temp, sizeof(uint));
		cursor += sizeof(uint);
		const char* path_temp = model->meshinfo[i]->route.c_str();
		memcpy(cursor, path_temp, sizeof(char)*temp);
		cursor += sizeof(char)*temp;
	}
	
	App->fs->SaveUnique(output, data, size, meta_path.c_str());
	



	/*std::string output;
	uint size = 0;
	char* data = new char[2];
	for (int i = 0; i < 2; ++i) 
	{
		memcpy(data, model->meshinfo[i]->route.c_str(), sizeof(char));
	}
	


	size = sizeof(uint);

	


	std::string temp = App->fs->GetFileName(path);
	std::string meta_path = LIBRARY_MODEL_FOLDER + temp + "_meta.json";
	App->fs->SaveUnique(output, data, size, meta_path.c_str());
*/
	//JSON_Value* value = json_parse_file(meta_path.c_str());
	//JSON_Object* object = json_object(value);
	//std::vector<meshInfo*>::iterator it = model->meshinfo.begin();
	//
	//
	//JSON_Value* leaf = json_value_init_object();
	//JSON_Object* leaf_obj = json_value_get_object(leaf);
	//json_serialize_to_file_pretty(leaf, meta_path.c_str());

	//json_object_dotset_string(leaf_obj, "Model.Name", temp.data());

	//json_object_dotset_number(leaf_obj, "Model.Meshes", model->meshinfo.size());

	//while(it < model->meshinfo.end())
	//{
	//	JSON_Array* arr = json_object_dotget_array(leaf_obj, "Model.PathMeshes");
	//	if (arr == nullptr) {
	//		JSON_Value* new_val = json_value_init_array();
	//		arr = json_value_get_array(new_val);

	//		json_object_dotset_value(leaf_obj, "Model.PathMeshes", new_val);
	//	}
	//	json_array_append_string(arr, (*it)->route.data());


	//	/*SetString(object, (*it)->route.c_str());
	//	json_parse_string((*it)->route.c_str());*/
	//	++it;
	//}


	//json_serialize_to_file_pretty(leaf, meta_path.c_str());


	//SerializeJSONFile(path, value);
	//--------------

	//char* bufferX;
	//uint sizeX;
	//char dataX*


	//uint ranges[2]; //error pq es constant
	//for (uint i = 0; i < model->meshinfo.size(); ++i)
	//{
	//	ranges[i] =(uint)model->meshinfo[i]->route.data();
	//	
	//}
	//LOG("%s", model->meshinfo[0]->route.data());
	//
	//uint size = sizeof(ranges);

	//for (int i = 0; i < model->meshinfo.size(); ++i)
	//{
	//	size += sizeof(uint) *(uint)model->meshinfo[i]->route.data();
	//}
	//

	//char* data = new char[size]; 
	//char* cursor = data;

	//uint bytes = sizeof(ranges);
	//memcpy(cursor, ranges, bytes);


	//for (int i = 0; i < model->meshinfo.size(); ++i) //routes of 
	//{
	//	cursor += bytes;
	//	bytes = sizeof(int) *(uint)model->meshinfo[i]->route.max_size(); //converts string to unsigned int
	//	memcpy(cursor, model->meshinfo[i]->route.c_str(), bytes);
	//}
	//

	//std::string temp = App->fs->GetFileName(path);
	//std::string test = LIBRARY_MODEL_FOLDER + temp + EXTENSION_MODEL_META;
	//LOG("tets %s", test.c_str());
	//std::string output;
	//App->fs->SaveUnique(output, data, size, test.c_str());



	//int kase = 0;
	//for (int i = 0; i < model->meshinfo.size(); ++i)
	//{
	//	if (i == model->meshinfo.size())
	//	{
	//	}
	//}
	//int my_array = model->meshinfo.size();
	//uint i = 0;
	//const char* ranges[2];
	//for (i; i < model->meshinfo.size(); ++i)
	//{
	//	ranges[i] = model->meshinfo[i]->route.c_str();
	//}

	//uint size = sizeof(ranges);

	//char* data = new char[size]; //Allocate
	//char* cursor = data;

	//uint bytes = sizeof(ranges); //Store ranges
	//memcpy(cursor, ranges, bytes);

}



