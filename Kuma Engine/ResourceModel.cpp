#include "ResourceModel.h"
#include "ModuleResource.h"
#include "ResourceTexture.h"
#include "ResourceMesh.h"
#include "ResourceMaterial.h"
#include "ModuleFileSystem.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

ResourceModel::ResourceModel(UID id, const char* base_path) : Resource(uid, Resource::Resource_Type::model)
{
	ref_path = base_path;
	uid = id;
	resource_name = base_path;
	resource_name = App->fs->GetFileName(base_path);
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
	uint n_mat = model->mNumMaterials;
	uint num = model->mRootNode->mNumChildren;
	
	//model->mMeshes[0]->mMaterialIndex;

	aiMaterial* mat;
	
	if (model->HasMeshes())
		LoadModelMeshes(model);

	if (model->HasTextures()) {}
		//LoadModelTextures()

	if (model->HasMaterials()) 
		LoadModelMaterials(model);

	
	LOG("");
}

void ResourceModel::LoadNodeMesh(aiNode* node, const aiScene* model)
{

	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		UID id = App->resources->GenerateUID();
		ImportMesh(node,model->mMeshes[node->mMeshes[i]],id);
		mesh_vec.push_back(id);
	}

	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		LoadNodeMesh(node->mChildren[i], model);

	}
}

void ResourceModel::LoadModelMaterials(const aiScene* model)
{
	for (uint i = 0; i < model->mNumMaterials; ++i)
	{
		UID id = App->resources->GenerateUID();
		ImportMaterial(model->mMaterials[i],id);

		mat_vec.push_back(id);
	}
}

bool ResourceModel::ImportMaterial(aiMaterial* material,UID id)
{

	ResourceMaterial* m = (ResourceMaterial*)App->resources->CreateNewResources(Resource::Resource_Type::material, id, ref_path);
	m->Import(material, ref_path);
	return false;
}




void ResourceModel::LoadModelMeshes(const aiScene* model) //2 
{

	//if(model->mNumMeshes >1)
	//	LoadNode(const)

	//ids.reserve(model->mNumMeshes); //to create extact size_t of the vector //not needed
	
	for (uint i =0 ; i < model->mRootNode->mNumMeshes; ++i)
	{

		UID id = App->resources->GenerateUID();
		ImportMesh(model->mRootNode, model->mMeshes[i], id);
		mesh_vec.push_back(id);
	}
	for (int i = 0; i < model->mRootNode->mNumChildren; ++i)
	{




		//cridar funcio recvursiva aqui aqui
		LoadNodeMesh(model->mRootNode->mChildren[i], model);

		//ImportMesh()

	}



}

void ResourceModel::ImportMesh(aiNode* node,aiMesh* mesh, UID id) //3
{
	ResourceMesh* m = (ResourceMesh*)(App->resources->CreateNewResources(Resource::Resource_Type::mesh, id, ref_path));

	mesh->mMaterialIndex;
	m->GetInfoF(mesh,node);

	if (m->SaveToMeta())
	{
		LOG("saved resource mesh to meta %i", m->GetUID());
		m->ReleaseFromMemory();
	}

	else LOG("Failed saving resource mesh to meta %i", m->GetUID());




	m->LoadMeta();
	
}

bool ResourceModel::SaveModelToMeta()
{
	std::string output;

//	for(int i =0;i < )
	return false;
}






