#pragma once

#include "Globals.h"
#include "Application.h"
#include "Module.h"
#include <map>
#include <vector>
#include "Resource.h"
enum class FileDropType;
class Resource;
class ResourceMesh;
class ResourceTexture;
class ResourceModel;
class ModuleResource : public Module
{
public:
	ModuleResource(Application* app, bool start_enabled = true);
	~ModuleResource();

	bool Init();
	bool Start();

	bool GenerateResourcesFromAssets();

	void LookIn(const char* path);

	bool CleanUp();
	void SaveResource();
	void LoadResource();
	//aixo quan estiguin creats els resources diria
	void ImportFile(const char* assets_path,Resource::Resource_Type type );
	Resource* CreateNewResources(Resource::Resource_Type type, UID meta_uid,const char* reference_path);

	bool DropTypeToResourceType(Resource::Resource_Type& r_type, FileDropType d_type);

	bool CheckifResourceExists(const char* ref_path);

	bool FindFileInDirectory(const char* dir, const char* filename, const char* extension);


	UID GenerateUID();
public:
	std::vector<Resource*> resources_vec;
	std::vector<Resource*> model_resources_vec;

};