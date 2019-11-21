#include "Globals.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleImporter.h"
//#include "ModuleResources.h"
#include "ModuleSerializeScene.h"
#include "PhysFS/include/physfs.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/types.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

#include "mmgr/mmgr.h"

using namespace std;

ModuleFileSystem::ModuleFileSystem(Application* app, const char* game_path, bool start_enabled) : Module(app, true)
{
	// needs to be created before Init so other modules can use it
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(base_path);
	SDL_free(base_path);

	// workaround VS string directory mess
	AddPath(".");

	if(0&&game_path != nullptr)
		AddPath(game_path);

	// Dump list of paths
	LOG("FileSystem Operations base is [%s] plus:", GetBasePath());
	LOG(GetReadPaths());

	// enable us to write in the game's dir area
	if(PHYSFS_setWriteDir(".") == 0)
		LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	// Make sure standard paths exist
	const char* dirs[] = {
		SETTINGS_FOLDER, ASSETS_FOLDER, LIBRARY_FOLDER,
		LIBRARY_AUDIO_FOLDER, LIBRARY_MESH_FOLDER,
		LIBRARY_MATERIAL_FOLDER, ASSETS_SCENE_FOLDER, LIBRARY_MODEL_FOLDER, 
		LIBRARY_TEXTURES_FOLDER, LIBRARY_ANIMATION_FOLDER, LIBRARY_STATE_MACHINE_FOLDER,
	};

	for (uint i = 0; i < sizeof(dirs)/sizeof(const char*); ++i)
	{
		if (PHYSFS_exists(dirs[i]) == 0)
			PHYSFS_mkdir(dirs[i]);
	}

	// Generate IO interfaces
	CreateAssimpIO();
	CreateBassIO();
}

// Destructor
ModuleFileSystem::~ModuleFileSystem()
{
	RELEASE(AssimpIO);
	RELEASE(BassIO);
	PHYSFS_deinit();
}

// Called before render is available
bool ModuleFileSystem::Init()
{
	LOG("Loading File System");
	bool ret = true;

	// Ask SDL for a write dir
	char* write_path = SDL_GetPrefPath(App->GetOrganizationName(), App->GetAppName());

	// Trun this on while in game mode
	//if(PHYSFS_setWriteDir(write_path) == 0)
		//LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());


	SDL_free(write_path);

	return ret;
}

// Called before quitting
bool ModuleFileSystem::CleanUp()
{
	//LOG("Freeing File System subsystem");

	return true;
}

// Add a new zip file or folder
bool ModuleFileSystem::AddPath(const char* path_or_zip)
{
	bool ret = false;

	if(PHYSFS_mount(path_or_zip, nullptr, 1) == 0)
		LOG("File System error while adding a path or zip: %s\n", PHYSFS_getLastError());
	else
		ret = true;

	return ret;
}

// Check if a file exists
bool ModuleFileSystem::Exists(const char* file) const
{

	return PHYSFS_exists(file) != 0;
}

// Check if a file is a directory
bool ModuleFileSystem::IsDirectory(const char* file) const
{
	return PHYSFS_isDirectory(file) != 0;
}

void ModuleFileSystem::CreateDirectory(const char* directory)
{
    PHYSFS_mkdir(directory);
}

void ModuleFileSystem::DiscoverFiles(const char* directory, vector<string> & file_list, vector<string> & dir_list) const
{
	char **rc = PHYSFS_enumerateFiles(directory);
	char **i;

	string dir(directory);

	for (i = rc; *i != nullptr; i++)
	{
		if(PHYSFS_isDirectory((dir+*i).c_str()))
			dir_list.push_back(*i);
		else
			file_list.push_back(*i);
	}

	PHYSFS_freeList(rc);
}

bool ModuleFileSystem::CopyFromOutsideFS(const char * full_path, const char * destination)
{
	// Only place we acces non virtual filesystem
 	bool ret = false;

    char buf[8192];
    size_t size;

	FILE* source = nullptr;
	fopen_s(&source,full_path, "rb");
	PHYSFS_file* dest = PHYSFS_openWrite(destination);

	if (source && dest)
	{
		while (size = fread_s(buf, 8192, 1, 8192, source))
			PHYSFS_write(dest, buf, 1, size);

		fclose(source);
		PHYSFS_close(dest);
		ret = true;

		LOG("File System copied file [%s] to [%s]", full_path, destination);
	}
	else
		LOG("File System error while copy from [%s] to [%s]", full_path, destination);

	return ret;
}

bool ModuleFileSystem::Copy(const char * source, const char * destination)
{
 	bool ret = false;

    char buf[8192];

	PHYSFS_file* src = PHYSFS_openRead(source);
	PHYSFS_file* dst = PHYSFS_openWrite(destination);

	PHYSFS_sint32 size;
	if (src && dst)
	{
		while (size = (PHYSFS_sint32) PHYSFS_read(src, buf, 1, 8192))
			PHYSFS_write(dst, buf, 1, size);

		PHYSFS_close(src);
		PHYSFS_close(dst);
		ret = true;

		LOG("File System copied file [%s] to [%s]", source, destination);
	}
	else
		LOG("File System error while copy from [%s] to [%s]", source, destination);

	return ret;
}

void ModuleFileSystem::SplitFilePath(const char * full_path, std::string * path, std::string * file, std::string * extension) const
{
	if (full_path != nullptr)
	{
		string full(full_path);
		NormalizePath(full);
		size_t pos_separator = full.find_last_of("\\/");
		size_t pos_dot = full.find_last_of(".");

		if (path != nullptr)
		{
			if (pos_separator < full.length())
				*path = full.substr(0, pos_separator + 1);
			else
				path->clear();
		}

		if (file != nullptr)
		{
			if (pos_separator < full.length())
				*file = full.substr(pos_separator + 1);
			else
				*file = full;
		}

		if (extension != nullptr)
		{
			if (pos_dot < full.length())
				*extension = full.substr(pos_dot + 1);
			else
				extension->clear();
		}


	
	}
}

// Flatten filenames to always use lowercase and / as folder separator
char normalize_char(char c)
{
	if (c == '\\')
		return '/';
	return tolower(c);
}

void ModuleFileSystem::NormalizePath(char * full_path) const
{
	int len = strlen(full_path);
	for (int i = 0; i < len; ++i)
	{
		if (full_path[i] == '\\')
			full_path[i] = '/';
		else
			full_path[i] = tolower(full_path[i]);
	}
}

void ModuleFileSystem::NormalizePath(std::string & full_path) const
{
	for (string::iterator it = full_path.begin(); it != full_path.end(); ++it)
	{
		if (*it == '\\')
			*it = '/';
		else
			*it = tolower(*it);
	}
}

std::string ModuleFileSystem::GetFileName(const char * file_name,bool extension)
{
	std::string name;
	std::string full_name(file_name);
	bool reached_dot = false;
	extension ? reached_dot = true : reached_dot = false;

	std::string::const_reverse_iterator item = full_name.crbegin();
	for (; item != full_name.crend(); ++item)
	{
		if (!reached_dot) {
			if (*item == '.')
				reached_dot = true;
		}
		else {
			if (*item == '/') {
				break;
			}
			else {
				name = *item + name;
			}
		}
	}
	return name;
}

std::string ModuleFileSystem::SubstractFromEnd(const char* file_name,const char* substract_to,int offset)
{

	std::string temp = file_name;
	size_t erase_meta = temp.rfind(substract_to);
	if(std::string::npos != erase_meta)
	{
			temp.erase(erase_meta+offset);
	}
	return temp;
}

std::string ModuleFileSystem::SubstracFromEndtoDigit(const char* file_name, int offset)
{
	std::string temp = file_name;
	
	std::vector<size_t>size_t_vec;
	for (int i = 0; i < 10; ++i)
	{
		size_t case_size = temp.rfind(std::to_string(i));
		if(case_size < 100)
		size_t_vec.push_back(case_size);
	}

	size_t final_size_to_erase;

	if (!size_t_vec.empty())
	{
		std::vector<size_t>::iterator it = size_t_vec.begin();

		final_size_to_erase = (*it);
		++it;

		while (it < size_t_vec.end())
		{
			if ((*it) > final_size_to_erase)
				final_size_to_erase = (*it); //get the largest size_t = the end number of string

			++it;
		}

		if (std::string::npos != final_size_to_erase)
		{
			temp.erase(final_size_to_erase + offset);
		}
	}
	return temp;
}

void ModuleFileSystem::SubstractFromBegin(std::string& path, const char* subs_to, int offset)
{


	size_t erase = path.find(subs_to);
	if (std::string::npos != erase)
	{
		path.erase(erase + offset);
	}
	
}


unsigned int ModuleFileSystem::Load(const char * path, const char * file, char ** buffer) const
{
	string full_path(path);
	full_path += file;
	return Load(full_path.c_str(), buffer);
}

// Read a whole file and put it in a new buffer
uint ModuleFileSystem::Load(const char* file, char** buffer) const
{
	uint ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openRead(file);

	if(fs_file != nullptr)
	{
		PHYSFS_sint32 size = (PHYSFS_sint32) PHYSFS_fileLength(fs_file);

		if(size > 0)
		{
			*buffer = new char[size];
			uint readed = (uint) PHYSFS_read(fs_file, *buffer, 1, size);
			if(readed != size)
			{
				LOG("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				RELEASE(buffer);
			}
			else
				ret = readed;
		}

		if(PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

// Read a whole file and put it in a new buffer
SDL_RWops* ModuleFileSystem::Load(const char* file) const
{
	char* buffer;
	int size = Load(file, &buffer);

	if(size > 0)
	{
		SDL_RWops* r = SDL_RWFromConstMem(buffer, size);
		if(r != nullptr)
			r->close = close_sdl_rwops;

		return r;
	}
	else
		return nullptr;
}

void * ModuleFileSystem::BassLoad(const char * file) const
{
	PHYSFS_file* fs_file = PHYSFS_openRead(file);

	if(fs_file == nullptr)
		LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return (void*) fs_file;
}

int close_sdl_rwops(SDL_RWops *rw)
{
	RELEASE_ARRAY(rw->hidden.mem.base);
	SDL_FreeRW(rw);
	return 0;
}

// Save a whole buffer to disk
uint ModuleFileSystem::Save(const char* file, const void* buffer, unsigned int size, bool append) const
{
	unsigned int ret = 0;

	bool overwrite = PHYSFS_exists(file) != 0;
	PHYSFS_file* fs_file = (append) ? PHYSFS_openAppend(file) : PHYSFS_openWrite(file);

	if(fs_file != nullptr)
	{
		uint written = (uint) PHYSFS_write(fs_file, (const void*)buffer, 1, size);
		if(written != size)
			LOG("File System error while writing to file %s: %s", file, PHYSFS_getLastError());
		else
		{
			if(append == true)
				LOG("Added %u data to [%s%s]", size, PHYSFS_getWriteDir(), file);
			//else if(overwrite == true)
				//LOG("File [%s%s] overwritten with %u bytes", PHYSFS_getWriteDir(), file, size);
			else if(overwrite == false)
				LOG("New file created [%s%s] of %u bytes", PHYSFS_getWriteDir(), file, size);

			ret = written;
		}

		if(PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s", file, PHYSFS_getLastError());

	return ret;
}

bool ModuleFileSystem::SaveUnique(string& name, const void * buffer, uint size, const char * path)
{
	char result[250];

	//sprintf_s(result, 250, "%s%s_%llu.%s", path, prefix, App->resources->GenerateNewUID(), extension);
	sprintf_s(result, 250, "%s", path);
	//NormalizePath(result);
	if (Save(result, buffer, size) > 0)
	{
		name = result;
		return true;
	}
	return false;
}

bool ModuleFileSystem::SaveUnique(string& name, const void* buffer, uint size, const char* path, const char* prefix, const char* extension)
{
	char result[250];

	sprintf_s(result, 250, "%s%s%s", path, prefix, extension);
	//NormalizePath(result);
	if (Save(result, buffer, size) > 0)
	{
		name = result;
		return true;
	}
	return false;
}

bool ModuleFileSystem::Remove(const char * file)
{
	bool ret = false;

	if (file != nullptr)
	{
		if (PHYSFS_delete(file) == 0)
		{
			LOG("File deleted: [%s]", file);
			ret = true;
		}
		else
			LOG("File System error while trying to delete [%s]: ", file, PHYSFS_getLastError());
	}

	return ret;
}

const char * ModuleFileSystem::GetBasePath() const
{
	return PHYSFS_getBaseDir();
}

const char * ModuleFileSystem::GetWritePath() const
{
	return PHYSFS_getWriteDir();
}

const char * ModuleFileSystem::GetReadPaths() const
{
	static char paths[512];

	paths[0] = '\0';

	char **path;
	for (path = PHYSFS_getSearchPath(); *path != nullptr; path++)
	{
		strcat_s(paths, 512, *path);
		strcat_s(paths, 512, "\n");
	}

	return paths;
}

const FileDropType & ModuleFileSystem::SearchExtension(const std::string & extern_path)
{
	std::string extension;
	SplitFilePath(extern_path.data(), nullptr, nullptr, &extension);

	FileDropType ext_type = FileDropType::UNKNOWN;

	if (extension == "FBX" || extension == "fbx")
		ext_type = FileDropType::MODEL3D;
	else if (extension == "PNG" || extension == "png" || extension == "jpg" || extension == "dds")
		ext_type = FileDropType::TEXTURE;

	else if (extension == "kumaScene" || extension == "kumascene")
		ext_type = FileDropType::SCENE;
	else
		LOG("Extension unknown!");


	return ext_type;

}

void ModuleFileSystem::ManageImportedFile(const char * first_path)
{
	LOG("Imported new file with path %s", first_path);

	std::string last_path;
	SplitFilePath(first_path, nullptr, &last_path);

	FileDropType f_type = SearchExtension(std::string(first_path));

	std::string aux_path = last_path;

	switch (f_type)
	{
	case FileDropType::MODEL3D:
		last_path = MODELS_FOLDER + last_path;
		break;
	case FileDropType::TEXTURE:
		last_path = TEXTURES_FOLDER + last_path;
		break;
	case FileDropType::SCENE:
		last_path = ASSETS_SCENE_FOLDER + last_path;
		break;

	default:
		break;
	}

	//Copy the file to assets folder if it doesn't exist
	std::string normalized_path = last_path;
	NormalizePath(normalized_path);
	if (!Exists(normalized_path.data()))
	{
		CopyFromOutsideFS(first_path, last_path.c_str());
	}

	switch (f_type)
	{
	case FileDropType::MODEL3D:
		App->importer->LoadModelFile(last_path.c_str());
		break;
	case FileDropType::TEXTURE:
		App->importer->LoadTextureFile(last_path.c_str());
		break;

	case FileDropType::SCENE:
		App->serialize->LoadScene(last_path.c_str());
		break;

	}

}
std::string ModuleFileSystem::GetModelMetaPath(const char * path) //aqui
{
	//TODO :/Suda dels ID i llegeix el nom
	//Estructura del nom : 345678283642473847_sida_model_meta.kuma

	std::string aha = path;
	SubstractFromBegin(aha,"_");
	std::string file;
	SplitFilePath(path, nullptr, &file);
	file = GetFileName(file.c_str());
	file = LIBRARY_MODEL_FOLDER + file + EXTENSION_MODEL_META;
	return file;
}
std::string ModuleFileSystem::GetTextureMetaPath(const char * path)
{
	std::string file;
	App->fs->SplitFilePath(path, nullptr, &file);
	file = App->fs->GetFileName(file.c_str());
	file = LIBRARY_TEXTURES_FOLDER + file + EXTENSION_TEXTURE_META;
	return file;
}
bool ModuleFileSystem::CheckIfExistingInMeta(const char* base_file_path,FileDropType& type)
{

	SearchExtension(base_file_path);
	std::string name = GetFileName(base_file_path);
	name = SubstractFromEnd(name.c_str(), ".", +1);

	switch (SearchExtension(base_file_path))
	{
	case FileDropType::MODEL3D:
		type = FileDropType::MODEL3D;

		name = LIBRARY_MODEL_FOLDER + name + EXTENSION_MODEL_META;
		NormalizePath(name);
		if (!Exists(name.c_str()))
		{
			return false;
		}
		else {
			return true;
		}
		break;
	case FileDropType::TEXTURE:
		type = FileDropType::TEXTURE;
		name = LIBRARY_MATERIAL_FOLDER + name + EXTENSION_TEXTURE_META;

		if (!Exists(name.c_str()))
		{
			return false;
		}
		else {
			return true;
		}
		break;
	case FileDropType::FOLDER:
		type = FileDropType::FOLDER;

		if (!Exists(name.c_str()))
		{
			return false;
		}
		else {
			return true;
		}
		break;
	case FileDropType::SCRIPT:
		type = FileDropType::SCRIPT;
		if (!Exists(name.c_str()))
		{
			return false;
		}
		else {
			return true;
		}
		break;
	case FileDropType::SCENE:
		type = FileDropType::SCENE;
		name = ASSETS_SCENE_FOLDER + name + EXTENSION_SCENE;
		if (!App->fs->Exists(name.c_str()))
		{
			//CREATE RESOURCE HERE
			LOG("");
			return false;
		}

		else
		{
			LOG("");
			return true;
		}
		break;
	case FileDropType::UNKNOWN:
		type = FileDropType::UNKNOWN;

		break;
	default:
		break;
	}
	
	return false;
}
	// -----------------------------------------------------
	// ASSIMP IO
	// -----------------------------------------------------
	

size_t AssimpWrite(aiFile* file, const char* data, size_t size, size_t chunks)
{
	PHYSFS_sint64 ret = PHYSFS_write((PHYSFS_File*)file->UserData, (void*)data, size, chunks);
	if(ret == -1)
		LOG("File System error while WRITE via assimp: %s", PHYSFS_getLastError());

	return (size_t) ret;
}

size_t AssimpRead(aiFile* file, char* data, size_t size, size_t chunks)
{
	PHYSFS_sint64 ret = PHYSFS_read((PHYSFS_File*)file->UserData, (void*)data, size, chunks);
	if(ret == -1)
		LOG("File System error while READ via assimp: %s", PHYSFS_getLastError());

	return (size_t) ret;
}

size_t AssimpTell(aiFile* file)
{
	PHYSFS_sint64 ret = PHYSFS_tell((PHYSFS_File*)file->UserData);
	if(ret == -1)
		LOG("File System error while TELL via assimp: %s", PHYSFS_getLastError());

	return (size_t) ret;
}

size_t AssimpSize(aiFile* file)
{
	PHYSFS_sint64 ret = PHYSFS_fileLength((PHYSFS_File*)file->UserData);
	if(ret == -1)
		LOG("File System error while SIZE via assimp: %s", PHYSFS_getLastError());

	return (size_t) ret;
}

void AssimpFlush(aiFile* file)
{
	if(PHYSFS_flush((PHYSFS_File*)file->UserData) == 0)
		LOG("File System error while FLUSH via assimp: %s", PHYSFS_getLastError());
}

aiReturn AssimpSeek(aiFile* file, size_t pos, aiOrigin from)
{
	int res = 0;

	switch (from)
	{
	case aiOrigin_SET:
		res = PHYSFS_seek((PHYSFS_File*)file->UserData, pos);
		break;
	case aiOrigin_CUR:
		res = PHYSFS_seek((PHYSFS_File*)file->UserData, PHYSFS_tell((PHYSFS_File*)file->UserData) + pos);
		break;
	case aiOrigin_END:
		res = PHYSFS_seek((PHYSFS_File*)file->UserData, PHYSFS_fileLength((PHYSFS_File*)file->UserData) + pos);
		break;
	}

	if(res == 0)
		LOG("File System error while SEEK via assimp: %s", PHYSFS_getLastError());

	return (res != 0) ? aiReturn_SUCCESS : aiReturn_FAILURE;
}

aiFile* AssimpOpen(aiFileIO* io, const char* name, const char* format)
{
	static aiFile file;

	file.UserData = (char*) PHYSFS_openRead(name);
	file.ReadProc = AssimpRead;
	file.WriteProc = AssimpWrite;
	file.TellProc = AssimpTell;
	file.FileSizeProc = AssimpSize;
	file.FlushProc= AssimpFlush;
	file.SeekProc = AssimpSeek;

	return &file;
}

void AssimpClose(aiFileIO* io, aiFile* file)
{
	if (PHYSFS_close((PHYSFS_File*)file->UserData) == 0)
		LOG("File System error while CLOSE via assimp: %s", PHYSFS_getLastError());
}

void ModuleFileSystem::CreateAssimpIO()
{
	RELEASE(AssimpIO);

	AssimpIO = new aiFileIO;
	AssimpIO->OpenProc = AssimpOpen;
	AssimpIO->CloseProc = AssimpClose;
}

aiFileIO * ModuleFileSystem::GetAssimpIO()
{
	return AssimpIO;
}

// -----------------------------------------------------
// BASS IO
// -----------------------------------------------------
/*
typedef void (CALLBACK FILECLOSEPROC)(void *user);
typedef QWORD (CALLBACK FILELENPROC)(void *user);
typedef DWORD (CALLBACK FILEREADPROC)(void *buffer, DWORD length, void *user);
typedef BOOL (CALLBACK FILESEEKPROC)(QWORD offset, void *user);

typedef struct {
	FILECLOSEPROC *close;
	FILELENPROC *length;
	FILEREADPROC *read;
	FILESEEKPROC *seek;
} BASS_FILEPROCS;
*/

void CALLBACK BassClose(void* file)
{
	if (PHYSFS_close((PHYSFS_File*)file) == 0)
		LOG("File System error while CLOSE via bass: %s", PHYSFS_getLastError());
}

QWORD CALLBACK BassLength(void* file)
{
	PHYSFS_sint64 ret = PHYSFS_fileLength((PHYSFS_File*)file);
	if(ret == -1)
		LOG("File System error while SIZE via bass: %s", PHYSFS_getLastError());

	return (QWORD) ret;
}

DWORD CALLBACK BassRead(void *buffer, DWORD len, void* file)
{
	PHYSFS_sint64 ret = PHYSFS_read((PHYSFS_File*)file, buffer, 1, len);
	if(ret == -1)
		LOG("File System error while READ via bass: %s", PHYSFS_getLastError());

	return (DWORD) ret;
}

BOOL CALLBACK BassSeek(QWORD offset, void* file)
{
	int res = PHYSFS_seek((PHYSFS_File*)file, offset);
	if(res == 0)
		LOG("File System error while SEEK via bass: %s", PHYSFS_getLastError());

	return (BOOL) res;
}

void ModuleFileSystem::CreateBassIO()
{
	RELEASE(BassIO);

	BassIO = new BASS_FILEPROCS;
	BassIO->close = BassClose;
	BassIO->length = BassLength;
	BassIO->read = BassRead;
	BassIO->seek = BassSeek;
}

BASS_FILEPROCS * ModuleFileSystem::GetBassIO()
{
	return BassIO;
}
