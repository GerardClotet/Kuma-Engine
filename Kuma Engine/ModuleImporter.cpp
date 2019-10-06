
#include "Globals.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "par_shapes.h"
#include "ModuleSceneIntro.h"
#include "Mesh.h"


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
	//LoadGeometry("../fbx/warrior.FBX");

	//Iterate the fbx list and call the Create function to create different meshes for a specific fbx
	//for (std::list<FBX*>::iterator item_fbx = fbx_list.begin(); item_fbx != fbx_list.end(); ++item_fbx)
	//{
	//	for (std::list<Mesh*>::iterator item_mesh = (*item_fbx)->mesh_list_fbx.begin(); item_mesh != (*item_fbx)->mesh_list_fbx.end(); ++item_mesh)
	//	{
	//		(*item_mesh)->CreateMesh();
	//	}
	//}

	return true;
}

update_status ModuleImporter::Update(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleImporter::PostUpdate(float dt)
{
	//Once all meshes are created, call the render function to draw all the shapes
	for (std::list<FBX*>::iterator item_fbx = fbx_list.begin(); item_fbx != fbx_list.end(); ++item_fbx)
	{
		for (std::list<Mesh*>::iterator item_mesh = (*item_fbx)->mesh_list_fbx.begin(); item_mesh != (*item_fbx)->mesh_list_fbx.end(); ++item_mesh)
		{
			(*item_mesh)->Render();
		}
	}
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
	FBX* fbx = new FBX();
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			Mesh* mesh = new Mesh();
			aiMesh* new_mesh = scene->mMeshes[i];

			mesh->num_vertex = new_mesh->mNumVertices;
			mesh->vertex = new float[mesh->num_vertex * 3];
			memcpy(mesh->vertex, new_mesh->mVertices, sizeof(float) * mesh->num_vertex * 3);
			LOG("New mesh with %d vertices", mesh->num_vertex);

			//copy normals
			if (new_mesh->HasNormals())
			{
				mesh->has_normals = true;
				mesh->num_normal = new_mesh->mNumVertices;
				mesh->normal = new float[mesh->num_normal * 3];
				memcpy(mesh->normal, new_mesh->mVertices, sizeof(float) * mesh->num_normal * 3);
				LOG("New mesh with %d normals", mesh->num_normal);
			}

	for (int i = 0; i < new_mesh->GetNumUVChannels(); ++i)
				{
					if (new_mesh->HasTextureCoords(i))
					{
						mesh->has_uvs = true;
						mesh->num_uvs = new_mesh->mNumVertices;
						mesh->uvs = new float[mesh->num_uvs * 2]; // 2 coords x,y each vertex
						memcpy(mesh->uvs, new_mesh->mTextureCoords[i], sizeof(float*) * mesh->num_uvs * 2);
						LOG("New mesh with %d uvs", mesh->num_uvs);
					}
				}
			//copy uvs
			if (new_mesh->HasTextureCoords(0)) {
				mesh->num_uvs = new_mesh->mNumVertices;
				mesh->uvs = new float[mesh->num_uvs * 2];
				for (uint i = 0; i < new_mesh->mNumVertices; ++i) {

					//There are two for each vertex
					memcpy(&mesh->uvs[i], &new_mesh->mTextureCoords[0][i].x, sizeof(float));
					memcpy(&mesh->uvs[i], &new_mesh->mTextureCoords[0][i].y, sizeof(float));
				}

			}

			//copy color
			if (new_mesh->HasVertexColors(0))
			{
				mesh->num_color = new_mesh->mNumVertices;
				mesh->color = new float[mesh->num_color * 4];

				for (uint i = 0; i < new_mesh->mNumVertices; ++i) //PROBABLY THIS IS WRONG, MAYBE IT NEEDS TO DO i+=4
				{
					memcpy(&mesh->color[i], &new_mesh->mColors[0][i].r, sizeof(float));
					memcpy(&mesh->color[i + 1], &new_mesh->mColors[0][i].g, sizeof(float));
					memcpy(&mesh->color[i + 2], &new_mesh->mColors[0][i].b, sizeof(float));
					memcpy(&mesh->color[i + 3], &new_mesh->mColors[0][i].a, sizeof(float));
				}
			}



			//copy faces
			if (new_mesh->HasFaces())
			{
				mesh->num_index = new_mesh->mNumFaces * 3;
				mesh->index = new uint[mesh->num_index]; // assume each face is a triangle

				for (uint j = 0; j < new_mesh->mNumFaces; ++j)
				{
					if (new_mesh->mFaces[j].mNumIndices != 3) {
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else {
						memcpy(&mesh->index[j * 3], new_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
					}
				}

			}

			SaveDebugData(mesh);

			mesh->CreateMesh();

			fbx->mesh_list_fbx.push_back(mesh); //pushback of the meshes
		
		}

		fbx_list.push_back(fbx); //pushback of the mesh list. Every FBX contains a lot of meshes

		aiReleaseImport(scene);
	}
	else LOG("Error loading scene %s", path);
}



void ModuleImporter::SaveDebugData(Mesh * mesh)
{
	geo_debug deb;
	for (int i = 0; i < mesh->num_index; i++)
	{
		//Triangle points
		uint index_01 = mesh->index[i] * 3;
		uint index_02 = mesh->index[i + 1] * 3;
		uint index_03 = mesh->index[i + 2] * 3;

		if (!mesh->vertex[index_01 + 1])
		{
			LOG("gGGGGGGGGG");
		}
		float3 p1 = { mesh->vertex[index_01], mesh->vertex[index_01 + 1], mesh->vertex[index_01 + 2] };
		float3 p2 = { mesh->vertex[index_02], mesh->vertex[index_02 + 1], mesh->vertex[index_02 + 2] };
		float3 p3 = { mesh->vertex[index_03], mesh->vertex[index_03 + 1], mesh->vertex[index_03 + 2] };

		//Calculate face center
		float C1 = (p1.x + p2.x + p3.x) / 3;
		float C2 = (p1.y + p2.y + p3.y) / 3;
		float C3 = (p1.z + p2.z + p3.z) / 3;

		//Calculate Face Normal
		float3 U = { p2 - p1 };
		float3 V = { p3 - p1 };

		float Nx = U.y*V.z - U.z*V.y;
		float Ny = U.z*V.x - U.x*V.z;
		float Nz = U.x*V.y - U.y*V.x;

		deb.tri_center.push_back({ C1, C2, C3 });
		deb.tri_normal.push_back({ Nx, Ny, Nz });

		i += 2;
	}
	m_debug.push_back(deb);
}

std::list<geo_debug> ModuleImporter::GetDebugInfo()
{
	return m_debug;
}
