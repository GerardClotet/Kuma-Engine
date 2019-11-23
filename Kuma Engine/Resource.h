#pragma once



#include "Application.h"
#include "Globals.h"


class Resource
{
	friend class ModuleResource;

public:

	enum Resource_Type
	{
		model=0,
		texture,
		mesh,
		material,
	};


	Resource(UID uid, Resource::Resource_Type type);
	virtual ~Resource();

	Resource::Resource_Type GetType() const;
	UID GetUID() const;

	uint CountResourceRefs()const;

	//Return true if the number of references to the resource is > 0
	bool IsLoadedToMemory();

	bool LoadToMemory();

protected:
	virtual bool LoadInMemory() = 0;
	virtual void ReleaseFromMemory();

protected:
	uint loaded;
	UID uid;
	Resource_Type type;
	const char* ref_path;
};