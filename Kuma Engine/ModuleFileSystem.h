#ifndef __MODULEFILESYSTEM_H__
#define __MODULEFILESYSTEM_H__

#include "Module.h"
#include <vector>

struct SDL_RWops;
int close_sdl_rwops(SDL_RWops *rw);

struct aiFileIO;



#include "Bass/include/bass.h"
//struct BASS_FILEPROCS;
enum class FileDropType {
	MODEL3D,
	TEXTURE,
	FOLDER,
	SCRIPT,

	UNKNOWN
};

class ModuleFileSystem : public Module
{
public:

	ModuleFileSystem(Application* app, const char* game_path = nullptr,  bool start_enabled = true);

	// Destructor
	~ModuleFileSystem();

	// Called before render is available
	bool Init();

	// Called before quitting
	bool CleanUp() override;

	// Utility functions
	bool AddPath(const char* path_or_zip);
	bool Exists(const char* file) const;
	bool IsDirectory(const char* file) const;
    void CreateDirectory(const char* directory);
	void DiscoverFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list) const;
	bool CopyFromOutsideFS(const char* full_path, const char* destination);
	bool Copy(const char* source, const char* destination);
	void SplitFilePath(const char* full_path, std::string* path, std::string* file = nullptr, std::string* extension = nullptr) const;
	void NormalizePath(char* full_path) const;
	void NormalizePath(std::string& full_path) const;
	std::string GetFileName(const char* file_name,bool extension = false);
	std::string SubstractFromEnd(const char* file_name, const char* substract_to,int offset =0);
	std::string SubstracFromEndtoDigit(const char* file_name, int offset = 0);
	void SubstractFromBegin(std::string& path, const char* subs_to, int offset = 0);
	// Open for Read/Write
	unsigned int Load(const char* path, const char* file, char** buffer) const;
	unsigned int Load(const char* file, char** buffer) const;
	SDL_RWops* Load(const char* file) const;
	void* BassLoad(const char* file) const;

	// IO interfaces for other libs to handle files via PHYSfs
	aiFileIO* GetAssimpIO();
	BASS_FILEPROCS* GetBassIO();

	unsigned int Save(const char* file, const void* buffer, unsigned int size, bool append = false) const;
	bool SaveUnique(std::string& output, const void* buffer, uint size, const char* path);
	bool SaveUnique(std::string& name, const void* buffer, uint size, const char* path, const char* prefix, const char* extension);
	bool Remove(const char* file);

	const char* GetBasePath() const;
	const char* GetWritePath() const;
	const char* GetReadPaths() const;

public: 
	const FileDropType& SearchExtension(const std::string& extern_path);
	void ManageImportedFile(const char* in_path);
	std::string GetModelMetaPath(const char* path);
	std::string GetTextureMetaPath(const char* path);

private:

	void CreateAssimpIO();
	void CreateBassIO();

private:

	aiFileIO* AssimpIO = nullptr;
	BASS_FILEPROCS* BassIO = nullptr;
};

#endif // __MODULEFILESYSTEM_H__
