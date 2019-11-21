#include "ResourceTexture.h"
#include "Application.h"
#include "ModuleTexture.h"
ResourceTexture::ResourceTexture(UID id): Resource(uid,Resource::Resource_Type::texture)
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
