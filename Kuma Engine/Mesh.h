#ifndef MESH_H
#define MESH_H

#include "Globals.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

class FBX;

class Mesh {
public:
	Mesh() {};
	~Mesh() {};

	void CreateMesh();
	void Render();
	void createCube(const vec3 &position, Color color = { 255,255,255,255 });

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
};

#endif // !MESH_H

