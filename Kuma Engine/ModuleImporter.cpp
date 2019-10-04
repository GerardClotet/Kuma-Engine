
#include "Globals.h"
#include "Application.h"
#include "ModuleImporter.h"

#include "ModuleSceneIntro.h"


#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) :Module(app, start_enabled)
{
}

ModuleImporter::~ModuleImporter()
{
}

void ModuleImporter::LoadGeometry(const char* path)
{

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		uint i = 0;

		for (aiMesh* (*new_mesh) = scene->mMeshes; i <= scene->mNumMeshes; ++i)
		{
			(*new_mesh)->mNormals;
			

			Mesh_Container mesh;
			mesh.num_vertex = (*new_mesh)->mNumVertices;
			mesh.vertex = new float[mesh.num_vertex * 3];
			memcpy(mesh.vertex, (*new_mesh)->mVertices, sizeof(float) * mesh.num_vertex * 3);
			LOG("New mesh with %d vertices", mesh.num_vertex);
			//Loading faces
			if ((*new_mesh)->HasFaces())
			{
				mesh.num_index = (*new_mesh)->mNumFaces * 3;
				mesh.index = new uint[mesh.num_index]; // assume each face is a triangle
				for (uint i = 0; i < (*new_mesh)->mNumFaces; ++i)
				{
					if ((*new_mesh)->mFaces[i].mNumIndices != 3)
						LOG("WARNING, geometry face with != 3 indices!");
					else
					{
						memcpy(&mesh.index[i * 3], (*new_mesh)->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}
			}

			//Loading Normals
			if ((*new_mesh)->HasNormals())
			{
				mesh.num_index = (*new_mesh)->mNumVertices;
				mesh.normals = new float[mesh.num_index * 3];
				memcpy(mesh.normals, (*new_mesh)->mNormals, sizeof(float) * mesh.num_normals * 3);
			}
			// buffer points
			glGenBuffers(1, &mesh.id_vertex);
			glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.num_vertex * 3, mesh.vertex, GL_STATIC_DRAW);

			// buffer index
			glGenBuffers(1, &mesh.id_index);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_index);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh.num_index * 3, mesh.index, GL_STATIC_DRAW);

			glGenBuffers(1,& mesh.id_normals);
			glBindBuffer(GL_ARRAY_BUFFER, mesh.id_normals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * mesh.num_normals, mesh.normals, GL_STATIC_DRAW);

			App->scene_intro->mesh_cont_list.push_back(mesh);
			LOG(" meshnumindex %u", mesh.num_index);
		
		}

	
	
		aiReleaseImport(scene);
	}
	else LOG("Error loading scene %s", path);
}

bool ModuleImporter::Init()
{
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	LOG("kpaso");
	return true;
}

bool ModuleImporter::Start()
{
	return true;
}

update_status ModuleImporter::Update(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleImporter::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}
