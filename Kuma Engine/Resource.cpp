#include "Resource.h"

Resource::Resource(UID uid, Resource::Resource_Type type) : uid(uid), type(type)
{
}

Resource::~Resource()
{
}

Resource::Resource_Type Resource::GetType() const
{
	return Resource_Type();
}

UID Resource::GetUID() const
{
	return UID();
}

uint Resource::CountResourceRefs() const
{
	return loaded;
}

bool Resource::IsLoadedToMemory() //If the n
{
	return loaded >0;
}

bool Resource::LoadToMemory()
{
	return false;
}

void Resource::ReleaseFromMemory()
{
}
