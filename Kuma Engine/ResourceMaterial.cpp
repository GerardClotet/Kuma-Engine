#include "ResourceMaterial.h"

#include "ModuleFileSystem.h"
#include "ModuleResource.h"
#include "ResourceTexture.h"
#include "Assimp/include/types.h"
#include "Assimp/include/material.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

ResourceMaterial::ResourceMaterial(UID id) : Resource(uid, Resource::Resource_Type::material)
{
	uid = id;
	mat_texture_metaPath = "id";
}

ResourceMaterial::~ResourceMaterial()
{
}

void ResourceMaterial::Import(const aiMaterial* material, const char* base_path)
{
	std::string path;
	path = App->fs->GetFileName(base_path, true);
	path = TEXTURES_FOLDER + path;
	//App->fs->SplitFilePath(base_path, &path, nullptr, nullptr);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color);
	material->Get(AI_MATKEY_COLOR_SPECULAR, specular_color);
	material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive_color);
	material->Get(AI_MATKEY_SHININESS, shininess);

	aiString texture_path;
	aiTextureMapping mapping;
	unsigned uvindex = 0;

	if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path, &mapping, &uvindex) == aiReturn_SUCCESS)
	{

		if (App->fs->HasDirectoryInPath(texture_path.C_Str()))
		{
			path = texture_path.C_Str();
			App->fs->EraseDotsFromBegin(path);
			App->fs->NormalizePath(path);
			path = App->fs->GetFileName(path.c_str(), true);
		}
		else path = texture_path.C_Str();
		
		path = TEXTURES_FOLDER + path;
		LOG("%s", path.c_str());

		std::string tpf = ImportTexture(path.c_str());
		std::string temp2 = App->fs->GetFileName(tpf.c_str(), false);

		if (strcmp(temp2.c_str(), "") == 0)
			tpf = "checkers";

		mat_texture_metaPath = tpf.c_str();
		SaveToMeta(tpf);
	}

}

std::string ResourceMaterial::ImportTexture(const char* path)
{
	UID id_t;
	std::string new_path;
	if (CheckTextureMeta(path, new_path)) //fer aixo

	{
		texture_id = App->resources->findIDbyTexPath(path);

		if (texture_id == 0)
			return "checkers";
		LOG("todo");
	}
		//Find UID by text path
	else
	{
		id_t = App->resources->GenerateUID();
		ResourceTexture* t = (ResourceTexture*)App->resources->CreateNewResources(Resource::Resource_Type::texture, id_t, path);

		//if properloaded
		if (t->ImportToMeta(path, new_path.c_str()) == false) //to polish but for now can't delete material, otherwise material index for meshes, can't be sync
		{
			checkers = true;
			texture_id = id_t;

			//tex_path =  new_path.c_str();
			return new_path;

		}

		//t->Load();
		t->ReleaseFromMemory();

		texture_id = id_t;

		return t->GetLibraryPath();
	}
	//return UID();
}

bool ResourceMaterial::CheckTextureMeta(const char* path, std::string& new_path)
{
	std::string temp;

	temp = App->fs->GetFileName(path, false);
	temp = LIBRARY_TEXTURES_FOLDER + temp + EXTENSION_TEXTURE_META;
	new_path = temp;
	if (App->fs->Exists(temp.c_str()))
		return true;


	return false;
}

void ResourceMaterial::SaveToMeta(std::string tpf)
{

	std::string output;

	uint size_path = 0;

	std::string tex_path = tpf;
	
	
	size_path += sizeof(char) * tex_path.size();
	size_path += sizeof(UID); //texture id
	size_path += sizeof(bool);

	uint size =  size_path;
	char* data = new char[size_path];
	char* cursor = data;

	
	uint temp = tex_path.size();
	memcpy(cursor, &temp, sizeof(char) * temp);
	cursor += sizeof(char) * temp;
	
	UID temp_id = texture_id;
	memcpy(cursor, &temp_id, sizeof(UID));
	cursor += sizeof(UID);

	bool temp_b = checkers;
	memcpy(cursor, &temp_b, sizeof(bool));
	cursor += sizeof(bool);

	std::string meta_path = LIBRARY_MATERIAL_FOLDER + std::to_string(uid) + EXTENSION_META_KUMA;

	App->fs->SaveUnique(output, data, size, meta_path.c_str());

}

void ResourceMaterial::LoadFromMeta()
{
}
