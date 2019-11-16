#pragma once


#include "Globals.h"
#include "Application.h"
#include "Module.h"

class SerializerR_JSON;
class ModuleSerializeScene : public Module
{
public:
	ModuleSerializeScene(Application* app, bool start_enabled = true);
	~ModuleSerializeScene();

	update_status Update(float dt);

	void LoadScene();
	void SaveScene(const char* path);

	SerializerR_JSON* json_serializer;
};