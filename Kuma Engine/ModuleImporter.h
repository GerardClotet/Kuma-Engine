#ifndef MODULEIMPORTER_H
#define MODULEIMPORTER_H

#include "Module.h"
#include "Globals.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

//struct Meshes //It stores all mesh data
//{
//	//vertex
//	uint id_vertex = 0;
//	uint num_vertex = 0;
//	float* vertex = nullptr;
//
//	//indices
//	uint id_index = 0;
//	uint num_index = 0;
//	uint* index = nullptr;
//
//	//normals
//	uint id_normals = 0;
//	uint num_normals = 0;
//	float* normals = nullptr;
//};

struct Mesh_Container
{
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = NULL;
	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = NULL;

	//normals
	uint id_normals = 0;
	uint num_normals = 0;
	float* normals = NULL;
};
class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();
	void LoadGeometry(const char* path);
	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	aiLogStream stream;


};




#endif // !MODULEIMPORTER_H

