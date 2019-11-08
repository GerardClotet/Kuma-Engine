#ifndef MODULEIMPORTER_H
#define MODULEIMPORTER_H

#include "Module.h"
#include "Globals.h"
#include <vector>
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "ImGui/imgui.h"
#include "MathGeoLib/include/MathGeoLib.h"


class vec3;
class GameObject;
struct Color;
enum class OBJECT_TYPE;
enum class GO_COMPONENT;
struct TexData;

struct meshInfo 
{
	uint id_index = 0;
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0;
	uint num_vertex = 0;
	float* vertex = nullptr;

	uint id_normal = 0;
	uint num_normal = 0;
	float* normal = nullptr;

	uint num_uvs = 0;
	uint id_uvs = 0;
	float* uvs = nullptr;

	bool has_normals = false;
	bool has_uvs = false;
	uint num_color = 0;
	uint id_color = 0;
	float* color = nullptr;
};

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();
	void LoadGeometry(const char* path);
	void LoadNode(const aiScene* importfile, aiNode* file_node, const char* name,GameObject* subparent);
	void LoadSingleMesh(const aiScene* importfile, const char* name, aiNode* node);
	bool LoadModelFile(const char* model_file);
	bool LoadTextureFile(const char* texture_file);
	bool Init();
	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	aiLogStream stream;
	ImVec4 wire_color = ImVec4(0.7, 0.0f, 1.f, 1.0f);
	ImVec4 fill_color = ImVec4(1.0f, 1.0f, 1.f, 1.0f);
	ImVec4 face_normal_color = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
	ImVec4 vertex_normal_color = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 vertex_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

	bool hasBeenLoaded = false;

	void getImportedName(const char* path);
	void LoadImportedMaterials(std::string path);
	void LoadTextureFromMaterial(std::string path,GameObject* game_object);//this could be in the same func as the one above but it's more efficient 
	std::string imported_name;
	std::string imported_route;
	TexData* texture;
private:
	std::string dropped_file_dir;
	SDL_Event event;
	bool dropped_file_done = false;
	

};




#endif // !MODULEIMPORTER_H

