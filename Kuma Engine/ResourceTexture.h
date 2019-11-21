#pragma once

#include "Resource.h"

class ResourceTexture : public Resource
{
	friend class ModuleTexture;

public:
	ResourceTexture(UID id);
	virtual ~ResourceTexture();

	bool LoadInMemory() override;
	void ReleaseFromMemory() override;
};