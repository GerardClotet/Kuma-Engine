#pragma once

#include "Resource.h"
#include "MathGeoLib/include/Math/float4x4.h"
struct aiScene; 
struct aiNode;
struct aiMesh;
struct aiMaterial;
class ResourceModel : public Resource
{
	

public:
	ResourceModel(UID id,const char* base_path);
	virtual ~ResourceModel();

	bool LoadInMemory() override;
	void ReleaseFromMemory() override;
	void LoadModelScene(const aiScene* model);

	void GenerateResource();
	void LoadModelMeshes(const aiScene* model);
	
	void LoadModel(const aiScene* model);
	//void GenerateMaterials(const aiMaterial*);
	void ImportMesh(aiNode* node,aiMesh* mesh,UID id);

	bool SaveModelToMeta();
	void LoadNodeMesh(aiNode* node, const aiScene* model);

	void LoadModelMaterials(const aiScene* model);
	bool ImportMaterial(aiMaterial* material,UID id);
public:

	std::vector<UID>mat_vec;
	std::vector<UID>mesh_vec;
};