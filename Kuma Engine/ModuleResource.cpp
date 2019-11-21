#include "ModuleResource.h"
#include "ModuleFileSystem.h"
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
	// GenerateInitMeta();
	return true;
}

bool ModuleResource::GenerateInitMeta()
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
		if (App->fs->CheckIfExistingInMeta(path_s.c_str(), d_type))
		{
			//EXISTS
			LOG("exists %s",path_s.c_str());
		}

		else
		{
			//CREATE RESOURCE, we have the type in d_type
			LOG("Doesnt Exist %s", path_s.c_str());

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
