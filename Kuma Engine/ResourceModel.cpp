#include "ResourceModel.h"
#include "ModuleResource.h"
#include "ResourceTexture.h"
#include "ResourceMesh.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/postprocess.h"
#include "RandomHelper.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

ResourceModel::ResourceModel(UID id, const char* base_path) : Resource(uid, Resource::Resource_Type::model)
{
	ref_path = base_path;
	uid = id;
	idk = uid;
	GenerateResource();
}

ResourceModel::~ResourceModel()
{
}

bool ResourceModel::LoadInMemory()
{
	return false;
}

void ResourceModel::ReleaseFromMemory()
{
}

void ResourceModel::LoadModelScene(const aiScene* model)
{
}

void ResourceModel::GenerateResource()
{
	const aiScene* scene = aiImportFile(ref_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene)
	{
		LoadModel(scene);


	}
}
void ResourceModel::LoadModel(const aiScene* model) //1
{
	//idea ultima hora fer un std::map per tindre guardat path assets lligat amb un id aixi se soluciona tot
	///genratemeshes
	//i despres generate node per tindre totes les seves texts / materials

	std::vector<UID> meshes;

	if (model->HasMeshes())
		LoadModelMeshes(model,meshes);

	if (model->HasTextures()) {}
		//LoadModelTextures()

	if (model->HasMaterials()) {}
		//LoadModelMaterials()

	uint num = model->mRootNode->mNumChildren;
	LOG("");
}



void ResourceModel::LoadModelMeshes(const aiScene* model, std::vector<UID>& ids) //2 
{

	//if(model->mNumMeshes >1)
	//	LoadNode(const)
	
	//ids.reserve(model->mNumMeshes); //to create extact size_t of the vector //not needed
	for (int i = 0; i < model->mNumMeshes; ++i)
	{

		uint32 kkk =  App->resources->GenerateUID();
		ImportMesh(model->mMeshes[i],kkk);
		ids.push_back(kkk);
		
		//ids.push_back(ResourceMesh::)
	}
}

void ResourceModel::ImportMesh(aiMesh* mesh, UID id) //3
{
	ResourceMesh* m = (ResourceMesh*)(App->resources->CreateNewResources(Resource::Resource_Type::mesh, id, ref_path));


	m->GetInfoF(mesh);

	if (m->SaveToMeta())
		LOG("saved resource mesh to meta %i", m->GetUID());

	else LOG("Failed saving resource mesh to meta %i", m->GetUID());


	m->ReleaseFromMemory();

	uid = idk;
}

bool ResourceModel::SaveModelToMeta()
{
	std::string output;

//	for(int i =0;i < )
	return false;
}

void ResourceModel::LoadNode(const aiScene* model, const aiNode* node, uint parent, const std::vector<UID>& meshes, const std::vector<UID>& mat)
{

	Node dst;

	dst.transform = reinterpret_cast<const float4x4&>(node->mTransformation);
	dst.name = node->mName.C_Str();
	dst.parent = parent;

}



