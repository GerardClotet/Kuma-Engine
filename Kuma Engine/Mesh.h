#ifndef MESH_H
#define MESH_H

#include "Globals.h"
//#include "ModuleTexture.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#define CHECKERS_HEIGHT 100
#define CHECKERS_WIDTH 100

class FBX;
class TexData;
class Mesh {
public:
	Mesh() {};
	~Mesh() {};

	void CreateMesh();
	void Render();
	bool gl_Short = false;
	bool gl_Int = false;
public:
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


	uint imageId;
	byte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

public:
	TexData* texture;
};

#endif // !MESH_H

