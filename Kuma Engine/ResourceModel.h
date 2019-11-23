#pragma once

#include "Resource.h"
#include "MathGeoLib/include/Math/float4x4.h"
struct aiScene; 
struct aiNode;
struct aiMesh;
class ResourceModel : public Resource
{
public:
	struct Node
	{
		std::string name;
		float4x4 transform = float4x4::identity;
		UID mesh;
		UID material;
		uint parent;
	};

	UID idk = 0;
public:
	ResourceModel(UID id,const char* base_path);
	virtual ~ResourceModel();

	bool LoadInMemory() override;
	void ReleaseFromMemory() override;
	void LoadModelScene(const aiScene* model);

	void GenerateResource();
	void LoadModelMeshes(const aiScene* model,std::vector<UID>& ids);
	
	void LoadNode(const aiScene* model, const aiNode* node,uint parent, const std::vector<UID>& meshes, const std::vector<UID>& mat);
	void LoadModel(const aiScene* model);
	//void GenerateMaterials(const aiMaterial*);
	void ImportMesh(aiMesh* mesh,UID id);

	bool SaveModelToMeta();
};