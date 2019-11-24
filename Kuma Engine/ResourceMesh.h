#pragma once

#include "Resource.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"


struct aiNode;
struct aiMesh;
struct m_debug
{
	std::vector<float3> centers_tri; //list of triangle centers
	std::vector<float3> normals_tri; //list of triangle normals
};
class ResourceMesh : public Resource
{
	friend class ModuleTexture;

public:
	ResourceMesh(UID id);
	virtual ~ResourceMesh();

	bool LoadInMemory() override;
	void ReleaseFromMemory() override;

	void GetInfoF(aiMesh* mesh,aiNode* node);
	void CreateFaceNormals();
	bool SaveToMeta();
	bool LoadMeta();
	void BindBuffers();
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

	//

	uint materialIndex;


	//

	float3 pos;
	float3 scale;
	Quat rot;

	//float3* posptr;
};