
#include "Globals.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "par_shapes.h"
#include "ModuleSceneIntro.h"
#include "Mesh.h"
#include "Cube.h"
#include "PanelConfig.h"
#include "ModuleUI.h"
#include "GameObject.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")



ModuleImporter::ModuleImporter(Application* app, bool start_enabled) :Module(app, start_enabled)
{
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Init()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return true;
}

bool ModuleImporter::Start()
{
	

	//CreateCube({ 5,0,-5 }, { 0,0,0.0f});



	return true;
}

update_status ModuleImporter::Update(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleImporter::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}



bool ModuleImporter::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();

	return true;
}


void ModuleImporter::LoadGeometry(const char* path)
{
	//Load all the shapes of the FBX loaded file  
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh* new_mesh = scene->mMeshes[i];

			App->scene_intro->AddGameObject("prove", OBJECT_TYPE::IMPORTER, new_mesh);
		}

		aiReleaseImport(scene);
	}
	else LOG("Error loading scene %s", path);
}



void ModuleImporter::SaveDebugData(Mesh * mesh)
{
	debug_mesh debItem;
	for (int i = 0; i < mesh->num_index; i++)
	{
		//Triangle points using indices and vertices
		uint index_01 = mesh->index[i] * 3;
		uint index_02 = mesh->index[i + 1] * 3;
		uint index_03 = mesh->index[i + 2] * 3;

		//Calculate the points of the triangle by using the vertex array and indices to find the exact vertex
		float3 p1 = { mesh->vertex[index_01], mesh->vertex[index_01 + 1], mesh->vertex[index_01 + 2] };
		float3 p2 = { mesh->vertex[index_02], mesh->vertex[index_02 + 1], mesh->vertex[index_02 + 2] };
		float3 p3 = { mesh->vertex[index_03], mesh->vertex[index_03 + 1], mesh->vertex[index_03 + 2] };

		//Calculate the center of the triangle C=(ax+bx+cx)/3
		float C1 = (p1.x + p2.x + p3.x) / 3;
		float C2 = (p1.y + p2.y + p3.y) / 3;
		float C3 = (p1.z + p2.z + p3.z) / 3;

		//Calculate two vectors by using the three points to calculate the cross product to get the normal
		float3 V = { p2 - p1 };
		float3 W = { p3 - p1 };

		//Cross product to get the normal(cross product gives a perpendicular vectorto the two given)
		float Nx = V.y*W.z - V.z*W.y;
		float Ny = V.z*W.x - V.x*W.z;
		float Nz = V.x*W.y - V.y*W.x;

		//Normalize vector
		vec3 normalizedVec = normalize({ Nx,Ny,Nz });
		


		debItem.centers_tri.push_back({ C1, C2, C3 });
		debItem.normals_tri.push_back({ normalizedVec.x, normalizedVec.y, normalizedVec.z });

		i += 2;
	}
	mesh_debug.push_back(debItem);
}

std::list<debug_mesh> ModuleImporter::GetDebugInfo()
{
	return mesh_debug;
}
