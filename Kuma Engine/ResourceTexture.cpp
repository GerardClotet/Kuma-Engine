#include "ResourceTexture.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleFileSystem.h"
ResourceTexture::ResourceTexture(UID id, const char* base_path): Resource(uid, Resource::Resource_Type::texture)
{
	ref_path = base_path; //Assets path
	uid = id;
	this->type = Resource::Resource_Type::texture;
	
}

ResourceTexture::~ResourceTexture()
{
}

bool ResourceTexture::LoadInMemory()
{
	//App->texture->LoadTexture
	Load();

	return false;
}

void ResourceTexture::ReleaseFromMemory()
{
	App->texture->RemoveTexture(data);
}

bool ResourceTexture::ImportToMeta(const char* original_path, const char* new_path)
{
	library_path = new_path;
	
	std::string base_temp = App->fs->GetFileName(original_path, true);
	
	data = App->texture->LoadTexture(original_path);
	if (data == nullptr)
		return false;
	//ExtractTexData();*/
	//Load();
	App->texture->SaveTextureTo(base_temp.c_str(), LIBRARY_TEXTURES_FOLDER);
	
	ReleaseFromMemory(); //take from memory assets textures

	data = App->texture->LoadTexture(library_path);
	ExtractTexData();
	//library_path = new_path;
	return true;
}

void ResourceTexture::ExtractTexData()
{
	
	width = data->width;
	height = data->height;
	offsetX = data->offsetX;
	offsetY = data->offsetY;
	name = data->name;
	name_extension = data->naem_extension;
	id = data->id;
	img = data->img;
	img_data = img_data;
}

void ResourceTexture::Load()
{
	 data = App->texture->LoadTexture(library_path);
	 ExtractTexData();
}

const char* ResourceTexture::GetLibraryPath()
{
	return library_path;
}
