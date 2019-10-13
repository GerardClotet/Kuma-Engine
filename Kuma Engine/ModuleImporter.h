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
#include "ImGui/imgui.h"
#include "MathGeoLib/include/MathGeoLib.h"


class Mesh;
class Cube;
class vec3;
struct Color;
enum class OBJECT_TYPE;


//----------------- FBX Class -----------------//

class FBX {
public:
	FBX() {};
	~FBX() {};
public:
	std::list<Mesh*> mesh_list_fbx;

};



struct debug_mesh {

	std::vector<float3> centers_tri; //list of triangle centers
	std::vector<float3> normals_tri; //list of triangle normals

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
	void CreateCube(const vec3& position, Color color);
	aiLogStream stream;

	void SaveDebugData(Mesh* mesh);
	std::list<debug_mesh> GetDebugInfo();
	ImVec4 wire_color = ImVec4(0.7, 0.0f, 1.f, 1.0f);
	ImVec4 fill_color = ImVec4(1.0f, 1.0f, 1.f, 1.0f);
	ImVec4 face_normal_color = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
	ImVec4 vertex_normal_color = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 vertex_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

	std::list<FBX*> fbx_list;
	std::list<Cube*> mesh_cube_list;

public:
	TexData* texture;
	Mesh* meshItem = new Mesh();

private:
	std::string dropped_file_dir;
	SDL_Event event;
	bool dropped_file_done = false;
	

	std::list<debug_mesh> mesh_debug;
	
//private:
//	void ImportVertex(Mesh*, aiMesh*);
//
//	void ImportNormals(Mesh*mesh, aiMesh*new_mesh);
//	void ImportUVS(Mesh*mesh, aiMesh*new_mesh);
//	void ImportVertexColors(Mesh*mesh, aiMesh*new_mesh);
//	void ImportFaces(Mesh*mesh, aiMesh*new_mesh);

};




#endif // !MODULEIMPORTER_H

