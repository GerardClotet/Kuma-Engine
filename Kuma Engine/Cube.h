#ifndef CUBE_H
#define CUBE_H

#include "Globals.h"

class Cube {

public:
	Cube() {};
	~Cube() {};

	void CreateMesh();
	void Render();

public:
	par_shapes_mesh_s* cube_mesh = nullptr;

private:
	uint my_id = 0;
	uint my_indices = 0;
	uint id_normal = 0;
	float* normal = nullptr;

};

#endif // !CUBE_H
