#pragma once

#include "Resource.h"

struct aiMaterial;

class ResourceMaterial : public Resource
{

public:
	ResourceMaterial(UID id);
	virtual ~ResourceMaterial();

	void Import(const aiMaterial* material, const char* base_path);
};
