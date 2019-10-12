
#ifndef MESH_COMPONENT_H
#define MESH_COMPOENENT_H


#include "Components.h"

class Component_Mesh : public Components
{
public:
	Component_Mesh();
	 ~Component_Mesh();

	bool Update();
	bool Enable();
	bool Disable();


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
};
#endif // !MESH_COMPONENT_H



