
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
	LoadGeometry("../fbx/warrior.FBX");

	//Iterate the fbx list and call the Create function to create different meshes for a specific fbx
	for (std::list<FBX*>::iterator item_fbx = fbx_list.begin(); item_fbx != fbx_list.end(); ++item_fbx)
	{
		for (std::list<Mesh*>::iterator item_mesh = (*item_fbx)->mesh_list_fbx.begin(); item_mesh != (*item_fbx)->mesh_list_fbx.end(); ++item_mesh)
		{
			(*item_mesh)->CreateMesh();
		}
	}

	return true;
}

update_status ModuleImporter::Update(float dt)
{
	//SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

	//dropped_file_done = false;
	//                        // Program loop
	//while (!dropped_file_done && SDL_PollEvent(&event)) {
	//	switch (event.type) {
	//	case (SDL_QUIT): {          // In case of exit
	//		dropped_file_done = true;
	//		break;
	//	}

	//	case (SDL_DROPFILE): {      // In case if dropped file
	//		dropped_file_dir = event.drop.file;

	//		// Shows directory of dropped file
	//		LOG("Directory file dropped: %s", dropped_file_dir);
	//		SDL_free(&dropped_file_dir);    // Free dropped_filedir memory
	//		break;
	//	}
	//	}
	//}
		
	


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


			fbx->mesh_list_fbx.push_back(mesh); //pushback of the meshes
		
		}
		fbx_list.push_back(fbx); //pushback of the mesh list. Every FBX contains a lot of meshes

		aiReleaseImport(scene);
	}
	else LOG("Error loading scene %s", path);
}


