#include "ModuleResource.h"
#include "ModuleFileSystem.h"
#include "Resource.h"
#include "ResourceTexture.h"
#include "ResourceMesh.h"
#include "ResourceModel.h"
#include "RandomHelper.h"
ModuleResource::ModuleResource(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleResource::~ModuleResource()
{
}

bool ModuleResource::Init()
{
	return true;
}

bool ModuleResource::Start()
{
	//posar aqui la func
	GenerateResourcesFromAssets();
	return true;
}

bool ModuleResource::GenerateResourcesFromAssets()
{


	const char* dir = ASSETS_FOLDER;
	std::vector<std::string> file_list;
	std::vector<std::string> dir_list;

	App->fs->DiscoverFiles(dir, file_list, dir_list);
	

	std::vector<std::string>::iterator f_it = file_list.begin();

	for (f_it; f_it < file_list.end(); ++f_it)
	{
		std::string path = dir + (*f_it);
		 
		App->fs->ManageImportedFile(path.c_str());
	}
	

	std::vector<std::string>::iterator d_it = dir_list.begin();


	while (d_it < dir_list.end())
	{
		std::string c_temp = ASSETS_FOLDER + (*d_it)+ "/";

		LookIn(c_temp.c_str());
		++d_it;
	}


	//App->fs->ManageImportedFile()


	return false;
}

void ModuleResource::LookIn(const char* path)
{

	std::vector<std::string> file_list;
	std::vector<std::string> dir_list;

	App->fs->DiscoverFiles(path, file_list, dir_list);
	std::vector<std::string>::iterator d_i = dir_list.begin();
	std::vector<std::string>::iterator f_i = file_list.begin();

	for (f_i; f_i < file_list.end(); ++f_i)
	{
		std::string path_s = path + (*f_i);

		FileDropType d_type;
		//if(CheckifResourceExists(path_s.c_str()))//Here chekcs the path and the reference name of the resources loaded
		if (App->fs->CheckIfExistingInMeta(path_s.c_str(), d_type)) //aqui haura de mirar les ids no els noms pero aixo es canviara més endavant
		{
			//EXISTS
			LOG("exists %s",path_s.c_str());
		}

		else
		{
			//CREATE RESOURCE, we have the type in d_type

		//	if(CheckifResourceExists(path_s.c_str()))//Here chekcs the path and the reference name of the resources loaded


			LOG("Doesnt Exist %s", path_s.c_str());
			
			
			Resource::Resource_Type r_type;
			if (DropTypeToResourceType(r_type, d_type))
			{
			
				CreateNewResources(r_type, GenerateUID(),path_s.c_str());

			}

			else LOG("file %s not supported as resource",path_s.c_str());

		}

	
	}


	while (d_i < dir_list.end())
	{
		std::string c_temp = path + (*d_i)+ "/";

		LookIn(c_temp.c_str());
		++d_i;
	}

}



bool ModuleResource::CleanUp()
{
	return false;
}

Resource* ModuleResource::CreateNewResources(Resource::Resource_Type type, UID uid,const char* base_path)
{
	Resource* resource = nullptr;
	


	switch (type)
	{
	case Resource::model:
		resource = (Resource*) new ResourceModel(uid,base_path);
		model_resources_vec.push_back(resource);
		resources_vec.push_back(resource);
		break;
	case Resource::texture:
		resource = (Resource*) new ResourceTexture(uid, base_path);
		resources_vec.push_back(resource);

		break;
	case Resource::mesh:
		resource = (Resource*) new ResourceMesh(uid);
		resources_vec.push_back(resource);

		break;

	case Resource::material:
		resource = (Resource*) new ResourceMesh(uid);
		resources_vec.push_back(resource);

		break;
	default:
		break;
	}
	return resource;
}

bool ModuleResource::DropTypeToResourceType(Resource::Resource_Type& r_type, FileDropType d_type)
{
	bool ret = true;
	switch (d_type)
	{
	case FileDropType::MODEL3D:

		r_type = Resource::Resource_Type::model;
		break;
	case FileDropType::TEXTURE:
		r_type = Resource::Resource_Type::texture;

		break;
	/*case FileDropType::FOLDER:
		break;
	case FileDropType::SCRIPT:
		break;
	case FileDropType::SCENE:
		break;
	case FileDropType::UNKNOWN:
		break;*/
	default:
		ret = false;
		LOG("resource not supported");
		break;
	}

	return ret;
}

bool ModuleResource::CheckifResourceExists(const char* ref_path) //check by reference Assets path
{
	std::vector<Resource*>::iterator it = resources_vec.begin();

	while (it < resources_vec.end())
	{
		if (strcmp(ref_path, (*it)->ref_path) == 0)
			return true;

		++it;
	}

	return false;
}

bool ModuleResource::FindFileInDirectory(const char* dir, const char* filename, const char* extension)
{
	std::vector<std::string> file_list;
	
	
	std::string temp = dir;
	temp = temp + filename;
	temp = temp + extension;

	if (App->fs->Exists(dir))
		return true;

	return false;
}

UID ModuleResource::GenerateUID()
{
	
	return GetRandomUID();
}
