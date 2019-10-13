
#ifndef MESH_COMPONENT_H
#define MESH_COMPOENENT_H


#include "Components.h"
#include "par_shapes.h"

enum class OBJECT_TYPE;
struct TexData;

class Component_Mesh : public Components
{
public:
	Component_Mesh(OBJECT_TYPE type);
	 ~Component_Mesh();

	virtual bool Update();
	virtual bool Enable();
	virtual bool Disable();
	void GenerateCube();
	void GenerateSphere();
	void GenerateCone();
	void GenerateCylinder();
	void GenerateDodecaedron();
	void CreateMesh();

public:
	par_shapes_mesh_s* par_mesh = nullptr;

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

	TexData* texture = nullptr;

public:
	bool gl_Short = false;
	bool gl_Int = false;
};
#endif // !MESH_COMPONENT_H



