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

	this->type = Resource::Resource_Type::model;
}

ResourceModel::~ResourceModel()
{
}



bool ResourceModel::LoadToMemory()
{
	LoadModelFromMeta();
	return true;
}

void ResourceModel::ReleaseFromMemory()
{
}

bool ResourceModel::IsLoadedToMemory()
{
	if (loaded > 0)
		return true;

	else return false;
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

	
	SaveModelToMeta();
	//Release it because no gamobject is pointing at it
	LoadModelFromMeta();

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

	m->materialIndex = mesh->mMaterialIndex;
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

	std::string a = "period";
//	for(int i =0;i < )
	std::string name = LIBRARY_MODEL_FOLDER + std::to_string(uid) + EXTENSION_MODEL_META;

	uint size = 0;
	size += sizeof(uint);
	for (int i = 0; i < mesh_vec.size(); ++i)
	{
		size += sizeof(UID);
	}

	size += sizeof(uint);

	//size += sizeof(char) * a.size();

	for (int i = 0; i < mat_vec.size(); ++i);
	{
		size += sizeof(UID);

	}


	char* data = new char[size];
	char* cursor = data;

	uint mt = mesh_vec.size();
	memcpy(cursor, &mt, sizeof(uint));
	cursor += sizeof(UID);

	std::vector<UID>::iterator it = mesh_vec.begin();
	while (it < mesh_vec.end() )
	{
		UID id = (*it);
		memcpy(cursor, &id, sizeof(UID));
		cursor += sizeof(UID);
		++it;
		
	}

	/*const char* p_t = a.c_str();

	memcpy(cursor, p_t, sizeof(char) * a.size());
	cursor += sizeof(char) * a.size();*/

	uint matt = mat_vec.size();
	memcpy(cursor, &matt, sizeof(uint));
	cursor += sizeof(uint);

	std::vector<UID>::iterator ir = mat_vec.begin();
	while (ir < mat_vec.end())
	{
		UID id = (*ir);
		memcpy(cursor, &id, sizeof(UID));
		cursor += sizeof(UID);
		++ir;

	}

	App->fs->SaveUnique(output, data, size, name.c_str());



	return false;
}

void ResourceModel::LoadModelFromMeta()
{
	std::string model_path = LIBRARY_MODEL_FOLDER + std::to_string(uid) + EXTENSION_MODEL_META;

	char* buffer = nullptr;
	App->fs->Load(model_path.c_str(), &buffer);
	char* cursor = buffer;

	uint mesh_size;
	memcpy(&mesh_size, cursor, sizeof(uint));
	cursor += sizeof(uint);

	std::vector<UID> temp_mesh_vec;
	for (int i = 0; i < mesh_size; ++i)
	{
		UID id;
		memcpy(&id, cursor, sizeof(UID));
		temp_mesh_vec.push_back(id);
		cursor += sizeof(UID);
	}

	//set mesh vec
	mesh_vec = temp_mesh_vec;

	
	uint mat_size;
	memcpy(&mat_size, cursor, sizeof(uint));
	cursor += sizeof(uint);

	std::vector<UID> temp_mat_vec;
	for (int i = 0; i < mat_size; ++i)
	{
		UID id;
		memcpy(&id, cursor, sizeof(UID));
		temp_mat_vec.push_back(id);
		cursor += sizeof(UID);
	}

	mat_vec = temp_mat_vec;

}






