#include "ResourceTexture.h"
#include "Application.h"
#include "ModuleTexture.h"
ResourceTexture::ResourceTexture(UID id, const char* base_path): Resource(uid, Resource::Resource_Type::texture)
{
	ref_path = base_path;
}

ResourceTexture::~ResourceTexture()
{
}

bool ResourceTexture::LoadInMemory()
{
	//App->texture->LoadTexture
	return false;
}

void ResourceTexture::ReleaseFromMemory()
{
}
