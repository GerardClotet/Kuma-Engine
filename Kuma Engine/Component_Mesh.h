
#ifndef MESH_COMPONENT_H
#define MESH_COMPOENENT_H


#include "Components.h"
#include "par_shapes.h"
#include <vector>
#include <list>
#include "MathGeoLib/include/MathGeoLib.h"

enum class OBJECT_TYPE;
class GameObject;
struct aiMesh;
struct TexData;

struct debug_mesh {

	std::vector<float3> centers_tri; //list of triangle centers
	std::vector<float3> normals_tri; //list of triangle normals

};

class Component_Mesh : public Components
{
public:
	Component_Mesh(OBJECT_TYPE type, GameObject* obj);
	Component_Mesh(OBJECT_TYPE type, aiMesh* mesh, GameObject* obj);
	 ~Component_Mesh();

	bool Update();
	bool Enable();
	bool Disable();
	void GenerateCube();
	void GenerateSphere();
	void GenerateCone();
	void GenerateCylinder();
	void GenerateDodecahedron();
	void GeneratePlane();
	void GenerateTorus();
	void GenerateImported(aiMesh* mesh);
	void CreateMesh();
	void CreateFaceNormals();
	void CreateVertexFaceNormals();
	void TranslateMesh(float pos[3]);
	std::list<debug_mesh> GetDebugInfo();
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

public:
	bool gl_Short = false;
	bool gl_Int = false;
	bool show_vertex = false;
	bool show_normals = false;
	bool show_face_normal = false;

public:
	GameObject* gameObject_Item = nullptr;

private:
	std::list<debug_mesh> mesh_debug;
	OBJECT_TYPE type;
	TexData* text = nullptr;
};
#endif // !MESH_COMPONENT_H



