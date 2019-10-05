#ifndef MESH_H
#define MESH_H

#include "Globals.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

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

#endif // !MESH_H

