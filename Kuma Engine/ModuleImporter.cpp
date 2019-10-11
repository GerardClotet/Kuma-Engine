
#include "Globals.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "par_shapes.h"
#include "ModuleSceneIntro.h"
#include "Mesh.h"
#include "Cube.h"
#include "PanelConfig.h"

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

		//Once all meshes are created, call the render function to draw all the shapes
	for (std::list<FBX*>::iterator item_fbx = fbx_list.begin(); item_fbx != fbx_list.end(); ++item_fbx)
	{
		for (std::list<Mesh*>::iterator item_mesh = (*item_fbx)->mesh_list_fbx.begin(); item_mesh != (*item_fbx)->mesh_list_fbx.end(); ++item_mesh)
		{

			if (App->ui->config_p->Getwireframe() && App->ui->config_p->GetFill())
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glPolygonOffset(1.0f, 0.375f); //test
				glColor4fv((float*)&wire_color);
				glLineWidth(1.0f);

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor4fv((float*)& fill_color);

				(*item_mesh)->Render();


			}
			else if (App->ui->config_p->GetFill())
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glColor4fv((float*)& fill_color);
				(*item_mesh)->Render();

			}
			if (App->ui->config_p->Getwireframe())
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glPolygonOffset(1.0f, 0.375f); //test
				glColor4fv((float*)& wire_color);
				glLineWidth(1.0f);
				(*item_mesh)->Render();

			}
		}
	}


	//Cube Render
	for (auto item_mesh = mesh_cube_list.begin(); item_mesh != mesh_cube_list.end(); ++item_mesh)
	{
		if (App->ui->config_p->Getwireframe() && App->ui->config_p->GetFill())
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glPolygonOffset(1.0f, 0.375f); //test
			glColor4fv((float*)&wire_color);
			glLineWidth(1.0f);

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glColor4fv((float*)& fill_color);

			(*item_mesh)->Render();


		}
		else if (App->ui->config_p->GetFill())
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glColor4fv((float*)& fill_color);
			(*item_mesh)->Render();

		}
		if (App->ui->config_p->Getwireframe())
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glPolygonOffset(1.0f, 0.375f); //test
			glColor4fv((float*)& wire_color);
			glLineWidth(1.0f);
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

void ModuleImporter::CreateCube(const vec3& position, Color color)
{
	Cube* cube = new Cube();
	cube->cube_mesh = par_shapes_create_cube();

	par_shapes_translate(cube->cube_mesh, position.x, position.y, position.z);

	par_shapes_unweld(cube->cube_mesh, true);
	par_shapes_compute_normals(cube->cube_mesh);
	
	cube->CreateMesh();
	
	mesh_cube_list.push_back(cube);

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


			//copy uvs
			for (int k = 0; k < new_mesh->GetNumUVChannels(); ++k)
			{
				if (new_mesh->HasTextureCoords(k)) {

					mesh->num_uvs = new_mesh->mNumVertices;
					mesh->uvs = new float[mesh->num_uvs * 2];
					uint j = 0;
					for (uint i = 0; i < new_mesh->mNumVertices; ++i) {
						mesh->has_uvs = true;
						//there are two for each vertex
						memcpy(&mesh->uvs[j], &new_mesh->mTextureCoords[k][i].x, sizeof(float));
						memcpy(&mesh->uvs[j + 1], &new_mesh->mTextureCoords[k][i].y, sizeof(float));
						j += 2;
					}

				}
			}
			//copy color
			if (new_mesh->HasVertexColors(0)) //need to put a var
			{
				mesh->num_color = new_mesh->mNumVertices;
				mesh->color = new float[mesh->num_color * 4];
				uint j = 0;
				for (uint i = 0; i < new_mesh->mNumVertices; ++i) 
				{
					memcpy(&mesh->color[j], &new_mesh->mColors[0][i].r, sizeof(float));
					memcpy(&mesh->color[j + 1], &new_mesh->mColors[0][i].g, sizeof(float)); //row var needed
					memcpy(&mesh->color[j + 2], &new_mesh->mColors[0][i].b, sizeof(float));
					memcpy(&mesh->color[j + 3], &new_mesh->mColors[0][i].a, sizeof(float));
					j += 4;
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


			mesh->gl_Int = true;
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
