#include "ResourceMesh.h"
#include "ModuleResource.h"
#include "Assimp/include/mesh.h"
#include "glmath.h"
#include "ModuleFileSystem.h"
ResourceMesh::ResourceMesh(UID id) : Resource(uid, Resource::Resource_Type::mesh)
{
	uid = id;
}

ResourceMesh::~ResourceMesh()
{
}

bool ResourceMesh::LoadInMemory()
{
	return false;
}

void ResourceMesh::ReleaseFromMemory()
{
	delete[] index;
	index = nullptr;

	delete[] vertex;
	vertex = nullptr;

	delete[] normal;
	normal = nullptr;

	delete[] uvs;

	uvs = nullptr;

	delete[] color;
	color = nullptr;
}



void ResourceMesh::GetInfoF(aiMesh* mesh)
{
	num_vertex = mesh->mNumVertices;
	vertex = new float[num_vertex * 3];

	memcpy(vertex, mesh->mVertices, sizeof(float) * num_vertex * 3);

	if (mesh->HasNormals())
	{
		has_normals = true;
		num_normal = mesh->mNumVertices;
		normal = new float[num_vertex * 3];
		memcpy(normal, mesh->mNormals, sizeof(aiVector3D) * num_normal);
	}

	for (int k = 0; k < mesh->GetNumUVChannels(); ++k)
	{
		if (mesh->HasTextureCoords(0))
		{
			num_uvs = mesh->mNumVertices;
			uvs = new float[num_uvs * 2];
			uint j = 0;
			for (uint i = 0; i < mesh->mNumVertices; ++i) {
				has_uvs = true;
				//there are two for each vertex
				memcpy(&uvs[j], &mesh->mTextureCoords[k][i].x, sizeof(float));
				memcpy(&uvs[j + 1], &mesh->mTextureCoords[k][i].y, sizeof(float));
				j += 2;
			}
			LOG("New FBX mesh with %d UVs", num_uvs);
		}
	}

	if (mesh->HasVertexColors(0))
	{
		num_color = mesh->mNumVertices;
		color = new float[num_color * 4];
		uint j = 0;
		for (uint i = 0; i < mesh->mNumVertices; ++i)
		{
			memcpy(&color[j], &mesh->mColors[0][i].r, sizeof(float));
			memcpy(&color[j + 1], &mesh->mColors[0][i].g, sizeof(float)); //row var needed
			memcpy(&color[j + 2], &mesh->mColors[0][i].b, sizeof(float));
			memcpy(&color[j + 3], &mesh->mColors[0][i].a, sizeof(float));
			j += 4;
		}
		LOG("New FBX mesh with %d colors", num_color);
	}

	//copy faces
	
	if (mesh->HasFaces())
	{
		bool createFaceNormals = false;
		num_index = mesh->mNumFaces * 3;
		index = new uint[num_index]; // assume each face is a triangle

		for (uint j = 0; j < mesh->mNumFaces; ++j)
		{
			if (mesh->mFaces[j].mNumIndices != 3) {
				LOG("WARNING, geometry face with != 3 indices!");
				createFaceNormals = true;
			}
			else {
				memcpy(&index[j * 3], mesh->mFaces[j].mIndices, 3 * sizeof(uint));
			}
		}
		LOG("New FBX mesh with %d index", num_index);


		if (!createFaceNormals)
			CreateFaceNormals();
	}
}

void ResourceMesh::CreateFaceNormals()
{
	m_debug  debItem;
	for (int i = 0; i < num_index; i++)
	{
		//Triangle points using indices and vertices
		uint index_01 = index[i] * 3;
		uint index_02 = index[i + 1] * 3;
		uint index_03 = index[i + 2] * 3;

		//Calculate the points of the triangle by using the vertex array and indices to find the exact vertex
		float3 p1 = { vertex[index_01], vertex[index_01 + 1], vertex[index_01 + 2] };
		float3 p2 = { vertex[index_02], vertex[index_02 + 1], vertex[index_02 + 2] };
		float3 p3 = { vertex[index_03], vertex[index_03 + 1], vertex[index_03 + 2] };

		//Calculate the center of the triangle C=(ax+bx+cx)/3
		float C1 = (p1.x + p2.x + p3.x) / 3;
		float C2 = (p1.y + p2.y + p3.y) / 3;
		float C3 = (p1.z + p2.z + p3.z) / 3;

		//Calculate two vectors by using the three points to calculate the cross product to get the normal
		float3 V = { p2 - p1 };
		float3 W = { p3 - p1 };

		//Cross product to get the normal(cross product gives a perpendicular vectorto the two given)
		float Nx = V.y * W.z - V.z * W.y;
		float Ny = V.z * W.x - V.x * W.z;
		float Nz = V.x * W.y - V.y * W.x;

		//Normalize vector
		vec3 normalizedVec = normalize({ Nx,Ny,Nz });



		debItem.centers_tri.push_back({ C1, C2, C3 });
		debItem.normals_tri.push_back({ normalizedVec.x, normalizedVec.y, normalizedVec.z });

		i += 2;
	}
}

bool ResourceMesh::SaveToMeta()
{
	uint ranges[5] = {
		num_vertex,
		num_index,
		num_normal,
		num_uvs,
		num_color,
	};

	uint size = sizeof(ranges)
		+ sizeof(uint) * num_vertex * 3
		+ sizeof(uint) * num_index
		+ sizeof(uint) * num_normal * 3
		+ sizeof(uint) * num_uvs * 2
		+ sizeof(uint) * num_color * 4;
		/*+ sizeof(uint)
		+ sizeof(char) * path_text.size();*/
	char* data = new char[size]; //Allocate
	char* cursor = data;


	uint bytes = sizeof(ranges); //Store ranges
	memcpy(cursor, ranges, bytes);


	cursor += bytes;
	bytes = sizeof(float) * num_vertex * 3;
	memcpy(cursor, vertex, bytes);

	cursor += bytes;//Store index;
	bytes = sizeof(uint) * num_index;
	memcpy(cursor, index, bytes);

	//----
	cursor += bytes;
	bytes = sizeof(float) * num_normal * 3;
	memcpy(cursor, normal, bytes);



	//---

	cursor += bytes;
	bytes = sizeof(float) * num_uvs * 2;
	memcpy(cursor, uvs, bytes);

	//--


	//---

	//----
	cursor += bytes;
	bytes = sizeof(float) * num_color * 4;
	memcpy(cursor, color, bytes);



	std::string name = LIBRARY_MESH_FOLDER + std::to_string(uid) + EXTENSION_META_KUMA;
	std::string output;
	bool ret = false;

	ret = App->fs->SaveUnique(output, data, size, name.c_str());

	
	return ret;
}

//USE only when reference count go from zero to one
bool ResourceMesh::LoadMeta()
{

	uint ranges[5] = {
		num_vertex,
		num_index,
		num_normal,
		num_uvs,
		num_color
	};

	char* buffer;
	std::string name = LIBRARY_MESH_FOLDER + std::to_string(uid) + EXTENSION_META_KUMA;
	uint testu = App->fs->Load(name.c_str(), &buffer);

	char* cursor = buffer;
	uint bytes = sizeof(ranges);

	memcpy(ranges, cursor, bytes);
	num_vertex = ranges[0];
	num_index = ranges[1];
	num_normal = ranges[2];
	num_uvs = ranges[3];
	num_color = ranges[4];

	cursor += bytes;
	bytes = sizeof(float) * num_vertex * 3;
	vertex = new float[num_vertex * 3];
	memcpy(vertex, cursor, bytes);

	cursor += bytes;
	bytes = sizeof(uint) * num_index;
	index = new uint[num_index];
	memcpy(index, cursor, bytes);

	cursor += bytes;
	bytes = sizeof(float) * num_normal * 3;
	normal = new float[num_normal * 3];
	memcpy(normal, cursor, bytes);


	cursor += bytes;
	bytes = sizeof(float) * num_uvs * 2;
	uvs = new float[num_uvs * 2];
	memcpy(uvs, cursor, bytes);



	cursor += bytes;
	bytes = sizeof(float) * num_color * 4;
	color = new float[num_color * 4];
	memcpy(color, cursor, bytes);

	return false;
}
