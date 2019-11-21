#pragma once

#include "Globals.h"
#include "Application.h"
#include "Module.h"
#include <map>
#include <vector>

class ModuleResource : public Module
{
public:
	ModuleResource(Application* app, bool start_enabled = true);
	~ModuleResource();

	bool Init();
	bool Start();

	bool GenerateInitMeta();

	void LookIn(const char* path);

	bool CleanUp();
	void SaveResource();
	void LoadResource();
};