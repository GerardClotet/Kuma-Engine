#include "ResourceMaterial.h"

ResourceMaterial::ResourceMaterial(UID id) : Resource(uid, Resource::Resource_Type::material)
{

}

void ResourceMaterial::Import(const aiMaterial* material, const char* base_path)
{
}
