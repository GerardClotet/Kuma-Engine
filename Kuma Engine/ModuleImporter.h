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
enum class GO_COMPONENT;


class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();
	void LoadGeometry(const char* path);
	void LoadNode(const aiScene* importfile, aiNode* file_node, const char* name);
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

	void getImportedName(const char* path);
	const char* test;
public:
	TexData* texture;
private:
	std::string dropped_file_dir;
	SDL_Event event;
	bool dropped_file_done = false;
	

};




#endif // !MODULEIMPORTER_H

