#pragma once

#include "Resource.h"
struct TexData;
class ResourceTexture : public Resource
{
	friend class ModuleTexture;

public:
	ResourceTexture(UID id,const char* base_path);
	virtual ~ResourceTexture();

	bool LoadInMemory() override;
	void ReleaseFromMemory() override;

	UID GetID() const { return uid; }
	uint GetWidth() const { return width; }
	uint GetHeight() const { return height; }

	bool ImportToMeta(const char* original_path,const char* new_path);

	void ExtractTexData();
	void Load();
public:

	TexData* data = nullptr;

	std::string name;
	std::string name_extension;
	int width = 0;
	int height = 0;
	int offsetX = 0;
	int offsetY = 0;
	GLuint id = 0;
	GLubyte img = 0;
	unsigned char* img_data = nullptr;
	byte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

	const char* library_path;
};