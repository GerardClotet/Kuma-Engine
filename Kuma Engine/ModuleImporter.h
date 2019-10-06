#ifndef MODULEIMPORTER_H
#define MODULEIMPORTER_H

#include "Module.h"
#include "Globals.h"
#include "Mesh.h"
#include <vector>
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

class Mesh;


//----------------- FBX Class -----------------//

class FBX {
public:
	FBX() {};
	~FBX() {};
public:
	std::list<Mesh*> mesh_list_fbx;

};



struct geo_debug {

	std::vector<float3> tri_center;
	std::vector<float3> tri_normal;

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
	void SaveDebugData(Mesh* mesh);
	std::list<geo_debug> GetDebugInfo();

private:
	std::list<FBX*> fbx_list;
	std::string dropped_file_dir;
	SDL_Event event;
	bool dropped_file_done = false;


	std::list<geo_debug> m_debug;
	
};




#endif // !MODULEIMPORTER_H

