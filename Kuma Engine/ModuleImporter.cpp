
#include "Globals.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "par_shapes.h"
#include "ModuleFileSystem.h"
#include "ModuleSceneIntro.h"
#include "ModuleTexture.h"
#include "PanelConfig.h"
#include "ModuleUI.h"
#include "GameObject.h"
#include "Components.h"
#include "Component_Material.h"
#include "Component_Mesh.h"
#include "RandomHelper.h"



#pragma comment (lib, "Assimp/libx86/assimp.lib")





ModuleImporter::ModuleImporter(Application* app, bool start_enabled) :Module(app, start_enabled)
{
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Init()
{
	bool ret = true;

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);



	return ret;
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

	
	std::string name;
	App->fs->SplitFilePath(path, nullptr, &name);
	name = App->fs->GetFileName(name.c_str());
	imported_name = name;

	
}

void ModuleImporter::LoadImportedMaterials(std::string path)
{
	if (App->scene_intro->selected_game_obj != nullptr)
	{
		if (App->scene_intro->selected_game_obj->type == OBJECT_TYPE::SUBPARENT )
			
		{
			std::vector<GameObject*>::iterator it = App->scene_intro->selected_game_obj->game_object_childs.begin();
			while (it != App->scene_intro->selected_game_obj->game_object_childs.end())
			{
				if ((*it)->material == nullptr)
					(*it)->AddComponent(GO_COMPONENT::MATERIAL);

				(*it)->material->ReadTexture(path.c_str());
				
				
				//CHECK IF THIS PATH ALREADY EXISTS, SO THE SAVE ONLY WILL BE DONE WHEN IT DOESN'T EXIST
				if (!App->fs->Exists(App->fs->GetTextureMetaPath(path.c_str()).c_str()))
					App->texture->SaveTextureTo(path.c_str(), LIBRARY_TEXTURES_FOLDER);
				
				++it;
			}
			return;
		}
		else if (App->scene_intro->selected_game_obj->mesh != nullptr)
		{
			if (App->scene_intro->selected_game_obj->material == nullptr)
				App->scene_intro->selected_game_obj->AddComponent(GO_COMPONENT::MATERIAL);

			App->scene_intro->selected_game_obj->material->ReadTexture(path.c_str());

			//CHECK IF THIS PATH ALREADY EXISTS, SO THE SAVE ONLY WILL BE DONE WHEN IT DOESN'T EXIST
			if (!App->fs->Exists(App->fs->GetTextureMetaPath(path.c_str()).c_str()))
				App->texture->SaveTextureTo(path.c_str(), LIBRARY_TEXTURES_FOLDER);

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
			
			App->scene_intro->selected_game_obj = go_subparent;
			LoadNode(scene, scene->mRootNode, path,go_subparent);
			SaveModelToMeta(path, model_info);
			go_subparent->GenerateParentBBox();
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
		go->SetBoundingBox();
		std::string path_tex;
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
					std::string path;
					if (App->fs->HasDirectoryInPath(texture_path.C_Str()))
					{

						path = texture_path.C_Str();
						App->fs->EraseDotsFromBegin(path);
						App->fs->NormalizePath(path);
						path = App->fs->GetFileName(path.c_str(), true);
					}
					else
					{
						path = texture_path.C_Str();
					}

					
					LOG("%s", texture_path.C_Str());
					path_tex = App->fs->GetTextureMetaPath(path.c_str());
						
					//-----------Get a texture of a fbx that has an associated material--------
					if (App->fs->Exists(path_tex.c_str()))
					{
						LoadTextureFromMaterial(path_tex,go);
						//if it exists, call de LoadTextureFromMeta
						//Call the LoadTextureFromMaterial with the path loaded in LoadTextureFromMeta
						LOG("IT EXISTS");
					}
					else
					{

						LoadTextureFromMaterial(/*imported_route +*/ path_tex, go);
						
						std::string temppp = imported_route + path;
						App->texture->SaveTextureTo(temppp.c_str(),path_tex.c_str() );
					}

				}
				else LOG("%s texture aiRETURN_FAILURE");
			}
			
		}

		SaveMeshToMeta(name, go->mesh->saveMeshinfo(), path_tex); //save it
		
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
	go->SetBoundingBox();
	App->scene_intro->selected_game_obj = go;
	std::string path_tex;
	unsigned int numat = importfile->mNumMaterials;

	if (importfile->HasMaterials())
	{
		//Texture
		for (uint m = 0; m < importfile->mNumMaterials; ++m) //no sense this here, just testing
		{
			aiMaterial* material = importfile->mMaterials[m];
			aiString texture_path;
			std::string tempo;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path) == aiReturn_SUCCESS)
			{
				tempo = texture_path.data;
				if (tempo != "*0")
				{
					LOG("%s", texture_path.C_Str());
					path_tex = App->fs->GetTextureMetaPath(texture_path.data);
					//-----------Get a texture of a fbx that has an associated material--------
					if (App->fs->Exists(path_tex.c_str()))
					{
						LoadTextureFromMaterial(path_tex, go);
						//if it exists, call de LoadTextureFromMeta
						//Call the LoadTextureFromMaterial with the path loaded in LoadTextureFromMeta
						LOG("IT EXISTS");
					}
					else
						LoadTextureFromMaterial(imported_route + texture_path.data, go);

				}

			}
			else LOG("%s texture aiRETURN_FAILURE");
		}

	}
	model_info = new modelInfo;
	SaveMeshToMeta(name, go->mesh->saveMeshinfo(), path_tex); //save it
	SaveModelToMeta(name, model_info);

}


bool ModuleImporter::LoadModelFile(const char * model_file)
{
	
	std::string path_meta = App->fs->GetModelMetaPath(model_file);
	if (App->fs->Exists(path_meta.c_str())) // need to look just for namemodel
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
		LoadImportedMaterials(std::string(texture_file));
		//LOADIMPORTEDMATERIALS. Since the texture is previously loaded, the save inside this function won't be executed
		//and the hierarchy will function when dragging a texture to a parent
		LOG("EXIST");

	}
	else
	{
		//It wasn't loaded before
		LoadImportedMaterials(std::string(texture_file));
	}
	return true;
}

void ModuleImporter::LoadTextureFromMeta(const char * path)
{

}

void ModuleImporter::SaveMeshToMeta(const char* path,meshInfo* mesh, std::string path_texture)
{
	mesh->path_text = path_texture;
	
	//TODO add more ranges DONE
	uint ranges[5] = {
		mesh->num_vertex,
		mesh->num_index ,
		mesh->num_normal,
		mesh->num_uvs,
		mesh->num_color,
	};

	uint size = sizeof(ranges)
		+ sizeof(uint) * mesh->num_vertex * 3
		+ sizeof(uint) * mesh->num_index
		+ sizeof(uint) * mesh->num_normal * 3
		+ sizeof(uint) * mesh->num_uvs * 2
		+ sizeof(uint) * mesh->num_color * 4
		+ sizeof(uint)
		+ sizeof(char) * mesh->path_text.size()
		+ sizeof(float) * 3
		+ sizeof(float) * 3
		+ sizeof(float) * 4;


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

	cursor += bytes;
	mesh->size_path_text = path_texture.size();
	bytes = sizeof(uint);
	memcpy(cursor, &mesh->size_path_text, bytes);

	if (mesh->size_path_text != 0)
	{
		cursor += bytes;
		bytes = sizeof(char)*path_texture.size();
		memcpy(cursor, mesh->path_text.c_str(), bytes);
	}

	for (int i = 0; i < 3; ++i)
	{
		cursor += bytes;
		bytes = sizeof(float);
		memcpy(cursor, &mesh->position[i], bytes);
	}
	for (int i = 0; i < 3; ++i)
	{
		cursor += bytes;
		bytes = sizeof(float);
		memcpy(cursor, &mesh->scale[i], bytes);
	}

	cursor += bytes;
	bytes = sizeof(float);
	memcpy(cursor, &mesh->rotation.x, bytes);

	cursor += bytes;
	bytes = sizeof(float);
	memcpy(cursor, &mesh->rotation.y, bytes);

	cursor += bytes;
	bytes = sizeof(float);
	memcpy(cursor, &mesh->rotation.z, bytes);

	cursor += bytes;
	bytes = sizeof(float);
	memcpy(cursor, &mesh->rotation.w, bytes);



	
	std::string name;
	if (mesh->name !="subparent") {
		 name =  LIBRARY_MESH_FOLDER + mesh->name + EXTENSION_META_KUMA;
		LOG("tets %s", name.c_str());

	}
	//this maybe is obsolete
	else
	{
		std::string temp = App->fs->GetFileName(path);
		name = LIBRARY_MESH_FOLDER + temp + EXTENSION_META_KUMA;
	}

	std::string output;
	App->fs->SaveUnique(output, data, size, name.c_str());
	LOG("output %s 1 %s", output.c_str(), output.c_str());
	mesh->route = name; //route to meta file

	model_info->meshinfo.push_back(mesh);
}

void ModuleImporter::LoadModelFromMeta(const char* original_path, const char* path)
{


	/*std::string UID_s =  App->fs->GetFileName(path);
	UID_s = App->fs->SubstractFromEnd(UID_s.c_str(), "_");
	
	UID_s = App->fs->SubstracFromEndtoDigit(UID_s.c_str());*/


	char* buffer = nullptr;
	uint testu = App->fs->Load(path, &buffer);


	char* cursor = buffer;
	uint num_meshes = 0;
	memcpy(&num_meshes, cursor, sizeof(uint)); 
	cursor += sizeof(uint); 

	GameObject* subparent = nullptr;
	if (num_meshes > 1)
	{
		subparent = App->scene_intro->CreateGameObject(nullptr, OBJECT_TYPE::SUBPARENT, App->fs->GetFileName(original_path));
		subparent->AddComponent(GO_COMPONENT::TRANSFORM, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f,0.0f });
		App->scene_intro->selected_game_obj = subparent;

	//	subparent->UUID = std::stoul(UID_s); //to string
	}

	for (int i = 0; i < num_meshes; ++i)
	{
		uint size_str = 0;
		memcpy(&size_str, cursor, sizeof(uint));
		cursor += sizeof(uint);
		char* path_temp = new char[size_str]; //
		memcpy(path_temp, cursor, sizeof(char)*size_str);
		std::string a_temp;
		a_temp = path_temp;//"name weird numebers"
		cursor += sizeof(char) * size_str; 

	

		size_t size_to_erase = a_temp.rfind("kuma");
		if (std::string::npos != size_to_erase +4)
		{
			a_temp.erase(size_to_erase+4);

			
		}
		std::string temporal = App->fs->GetFileName(a_temp.c_str());

		GameObject* child = App->scene_intro->CreateGameObject(subparent, OBJECT_TYPE::IMPORTER, App->fs->SubstractFromEnd(temporal.c_str(),EXTENSION_META));

		child->AddComponent(GO_COMPONENT::MESH, LoadMeshFromMeta(a_temp.c_str()));

		child->AddComponent(GO_COMPONENT::TRANSFORM, child->mesh->position, child->mesh->scale, child->mesh->rotation);
		child->SetBoundingBox();

		if (std::string(child->mesh->path_texture_associated_meta) != "")
		{
			child->AddComponent(GO_COMPONENT::MATERIAL);
			LOG("esto %s", child->mesh->path_texture_associated_meta);
			std::string temp_str = child->mesh->path_texture_associated_meta;
			child->material->ReadTexture(temp_str.c_str());
		}
		
		
		
		
	}

	//GUARRADA
	if(subparent!=nullptr)
		subparent->GenerateParentBBox();
	
}

//void ModuleImporter::SaveTextureToMeta(const char * path)
//{
//	std::string output;
//	ILuint size;
//	ILubyte *data;
//	
//	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
//	size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
//	if (size > 0) {
//		data = new ILubyte[size]; // allocate data buffer
//		iluFlipImage();
//		if (ilSaveL(IL_DDS, data, size) > 0)
//		{
//			// Save to buffer with the ilSaveIL function
//			std::string temp = App->fs->GetFileName(path);
//			App->fs->SaveUnique(output, data, size, LIBRARY_TEXTURES_FOLDER, temp.c_str(), ".dds");
//		}
//		RELEASE_ARRAY(data);
//	}
//}

meshInfo* ModuleImporter::LoadMeshFromMeta(const char* path)
{
		meshInfo* mesh = new meshInfo;

		//TODO :/read the UUID from memory

		uint ranges[5] = {
			mesh->num_vertex,
			mesh->num_index,
			mesh->num_normal,
			mesh->num_uvs,
			mesh->num_color
	
		};

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

		cursor += bytes;
		bytes = sizeof(uint);
		mesh->size_path_text = 0;
		memcpy(&mesh->size_path_text, cursor, bytes);

		if (mesh->size_path_text != 0)
		{
			cursor += bytes;
			bytes = sizeof(char)*mesh->size_path_text;
			char* temp_text_path = new char[mesh->size_path_text];

			memcpy(temp_text_path, cursor, bytes);

			
			LOG("memcpy %s", temp_text_path);
			std::string tmp_ext = EXTENSION_TEXTURE_META;
			
			mesh->path_text = App->fs->SubstractFromEnd(temp_text_path, EXTENSION_TEXTURE_META, tmp_ext.size()/*4extension size*/);
		}

		mesh->route = path;


		for (int i = 0; i < 3; ++i)
		{
			cursor += bytes;
			bytes = sizeof(float);
			float* t_f = new float;
			memcpy(t_f, cursor, bytes);

			//float& si;
			mesh->position[i] = t_f[0];
			LOG("pos %i %f", i, mesh->position[i]);

		}
		for (int i = 0; i < 3; ++i)
		{
			cursor += bytes;
			bytes = sizeof(float);
			float* t_f = new float;
			memcpy(t_f, cursor, bytes);

			//float& si;
			mesh->scale[i] = t_f[0];
			LOG("scale %i %f", i, mesh->scale[i]);

		}

		cursor += bytes;
		bytes = sizeof(float);
		float* t_x = new float;
		memcpy(t_x, cursor, bytes);

		mesh->rotation.x = t_x[0];
		LOG("rot x %f", mesh->rotation.x);

		cursor += bytes;
		bytes = sizeof(float);
		float* t_y = new float;
		memcpy(t_y, cursor, bytes);

		mesh->rotation.y = t_y[0];
		LOG("rot y %f", mesh->rotation.y);

		cursor += bytes;
		bytes = sizeof(float);
		float* t_z = new float;
		memcpy(t_z, cursor, bytes);

		mesh->rotation.z = t_z[0];
		LOG("rot z %f", mesh->rotation.z);

		cursor += bytes;
		bytes = sizeof(float);
		float* t_f = new float;
		memcpy(t_f, cursor, bytes);

		mesh->rotation.w = t_f[0];
		LOG("rot w %f", mesh->rotation.w);

		return mesh;
}

void ModuleImporter::SaveModelToMeta(const char* path,modelInfo* model)
{
	

	std::string output;
	std::string temp = App->fs->GetFileName(path);
	std::string meta_path = LIBRARY_MODEL_FOLDER  + temp + EXTENSION_MODEL_META;
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
	
	

}

TexData * ModuleImporter::LoadTextureDevil(const char * path)
{
	return nullptr;
}



