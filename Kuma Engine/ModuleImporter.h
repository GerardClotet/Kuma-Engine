#ifndef MODULEIMPORTER_H
#define MODULEIMPORTER_H

#include "Module.h"
#include "Globals.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"


//----------------- Mesh -----------------//
class Mesh {
public:
	Mesh() {};
	~Mesh() {};

	void CreateMesh();
	void Render();

public:
	uint id_index = 0;
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0;
	uint num_vertex = 0;
	float* vertex = nullptr;

};

//----------------- FBX -----------------//

class FBX {
public:
	FBX() {};
	~FBX() {};
public:
	std::list<Mesh*> mesh_list;

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
	update_status PostUpdate(float dt);
	bool CleanUp();

	aiLogStream stream;
	

private:
	std::list<FBX*> fbx_list;
	
};




#endif // !MODULEIMPORTER_H

