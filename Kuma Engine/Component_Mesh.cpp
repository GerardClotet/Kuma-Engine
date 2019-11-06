#include "Component_Mesh.h"
#include "GameObject.h"
#include "Component_Material.h"
#include "Component_Transform.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "ModuleUI.h"
#include "ModuleTexture.h"

#include "Cube.h"


Component_Mesh::Component_Mesh(OBJECT_TYPE type, GameObject* obj) : Components()
{
	name = "mesh";
	this->type = type;
	this->gameObject_Item = obj;
	comp_type = GO_COMPONENT::MESH;
	switch (type)
	{
	case OBJECT_TYPE::PARENT:
		break;
	case OBJECT_TYPE::SUBPARENT:
		break;
	case OBJECT_TYPE::CUBE:
		GenerateCube();
		break;
	case OBJECT_TYPE::SPHERE:
		GenerateSphere();
		break;
	case OBJECT_TYPE::CYLINDER:
		GenerateCylinder();
		break;
	case OBJECT_TYPE::CONE:
		GenerateCone();
		break;
	case OBJECT_TYPE::PLANE:
		GeneratePlane();
		break;
	case OBJECT_TYPE::IMPORTER:
		//gl_int=true;
		break;
	case OBJECT_TYPE::DODECAHEDRON:
		GenerateDodecahedron();
		break;

	case OBJECT_TYPE::TORUS:
		GenerateTorus();
	default:
		break;
	}
}

Component_Mesh::Component_Mesh(OBJECT_TYPE type, aiMesh * mesh, GameObject* obj, aiNode* node)
{
	comp_type = GO_COMPONENT::MESH;
	this->type = type;
	this->gameObject_Item = obj;
	GenerateImported(mesh, node);
}

Component_Mesh::~Component_Mesh()
{
	LOG("deleted mesh");
	mesh_debug.clear();
	gameObject_Item = nullptr;
	index = nullptr;
	vertex = nullptr;
	normal = nullptr;
	uvs = nullptr;
	color = nullptr;
}
bool Component_Mesh::Update()
{
	//Read buffers and draw the shapes
	glPushMatrix();
	glMultMatrixf(gameObject_Item->transform->global_transformation.ptr());  // we need to pass here our global matrix

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	if (gameObject_Item->material != nullptr && gameObject_Item->material->isTextureEnable)
		glEnable(GL_TEXTURE_2D);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);

	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);

	//Read and Draw normals buffers
	if (has_normals)
	{
		glBindBuffer(GL_ARRAY_BUFFER, id_normal);
		glNormalPointer(GL_FLOAT, 0, NULL);

	}


	//Read and Draw UVS buffers
	if (has_uvs)
	{

		if (gameObject_Item->material && gameObject_Item->material->setTexture)
		{
			text = gameObject_Item->material->GetTexture();
		}
		if (text != nullptr)
			glBindTexture(GL_TEXTURE_2D, text->id);

		//glGenerateMipmap(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, id_uvs);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	}




	if (gl_Int)
		glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_INT, NULL);

	else if (gl_Short)
		glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_SHORT, NULL);


	glDisable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();

	//draw normals
	if (show_normals)
	{
		glLineWidth(2.5f);
		glBegin(GL_LINES);

		glColor4fv((float*)& App->importer->vertex_normal_color);

		for (int i = 0; i < num_normal * 3; i += 3)
		{
			vec3 normalVec = normalize({ normal[i], normal[i + 1], normal[i + 2] });
			glVertex3f(vertex[i], vertex[i + 1], vertex[i + 2]);
			glVertex3f(vertex[i] + normalVec.x, vertex[i + 1] + normalVec.y, vertex[i + 2] + normalVec.z);
		}
		glColor3f(255.0f, 255.0f, 255.0f);
		glEnd();
	}


	//draw vertex
	if (show_vertex)
	{
		glPointSize(2.5f);
		glBegin(GL_POINTS);
		glColor4fv((float*)& App->importer->vertex_color);
		for (int i = 0; i < num_vertex * 3; i += 3)
		{
			glVertex3f(vertex[i], vertex[i + 1], vertex[i + 2]);
		}

		
		glEnd();
	}

	//draw face normal
	if (show_face_normal)
	{
		std::list<debug_mesh> mesh_debug = GetDebugInfo();
		std::list<debug_mesh>::iterator deb = mesh_debug.begin();

		while (deb != mesh_debug.end())
		{
			//Check if face normals is activated
			if (show_face_normal) {
				glPointSize(2.5f);
				glBegin(GL_LINES);

				glColor4fv((float*)& App->importer->face_normal_color);

				for (int i = 0; i < deb->normals_tri.size(); i++)
				{
					//Draw the normals
					glVertex3f(deb->centers_tri[i].x, deb->centers_tri[i].y, deb->centers_tri[i].z);
					glVertex3f(deb->centers_tri[i].x + deb->normals_tri[i].x, deb->centers_tri[i].y + deb->normals_tri[i].y,
						deb->centers_tri[i].z + deb->normals_tri[i].z);
				}
				glColor3f(255.0f, 255.0f, 255.0f);
				glEnd();
			}

			deb++;
		}

	}

	return true;
}

bool Component_Mesh::Enable()
{
	enabled = true;
	return enabled;
}
bool Component_Mesh::Disable()
{
	enabled = false;
	return enabled;
}

void Component_Mesh::GenerateCube()
{
	par_mesh = par_shapes_create_cube();

	par_shapes_translate(par_mesh, 0.0f, 0.0f,0.0f);
	par_shapes_unweld(par_mesh, true);
	par_shapes_compute_normals(par_mesh);

	num_vertex = par_mesh->npoints;
	num_index = par_mesh->ntriangles * 3;

	vertex = new float[num_vertex * 3];
	index = new uint[num_index * 3];

	memcpy(vertex, par_mesh->points, sizeof(float) * num_vertex * 3);
	memcpy(index, par_mesh->triangles, sizeof(PAR_SHAPES_T) * num_index);

	num_normal = par_mesh->npoints;
	normal = new float[num_vertex * 3];
	memcpy(normal, par_mesh->normals, sizeof(float) * num_vertex * 3);


	num_uvs = par_mesh->npoints;
	uvs = new float[num_uvs * 2];

	gl_Short = true;
	has_normals = true;
	has_uvs = true;
	LOG("New Cube mesh with %d vertices", num_vertex);
	LOG("New Cube mesh with %d index", num_index);
	LOG("New Cube mesh with %d normals", num_normal);
	LOG("New Cube mesh with %d UVs", num_uvs);
	CreateVertexFaceNormals();
	CreateMesh();

	par_shapes_free_mesh(par_mesh);

}

void Component_Mesh::GenerateSphere()
{
	par_mesh = par_shapes_create_subdivided_sphere(3);
	par_shapes_translate(par_mesh, 0.0f, 0.0f, 0.0f);
	par_shapes_unweld(par_mesh, true);
	par_shapes_compute_normals(par_mesh);

	num_vertex = par_mesh->npoints;
	num_index = par_mesh->ntriangles * 3;

	vertex = new float[num_vertex * 3];
	index = new uint[num_index * 3];

	memcpy(vertex, par_mesh->points, sizeof(float) * num_vertex * 3);
	memcpy(index, par_mesh->triangles, sizeof(PAR_SHAPES_T) * num_index);

	num_normal = par_mesh->npoints;
	normal = new float[num_vertex * 3];
	memcpy(normal, par_mesh->normals, sizeof(float) * num_vertex * 3);


	num_uvs = par_mesh->npoints;
	uvs = new float[num_uvs * 2];

	gl_Short = true;
	has_normals = true;
	has_uvs = true;

	LOG("New Sphere mesh with %d vertices", num_vertex);
	LOG("New Sphere mesh with %d index", num_index);
	LOG("New Sphere mesh with %d normals", num_normal);
	LOG("New Sphere mesh with %d UVs", num_uvs);
	
	CreateVertexFaceNormals();
	CreateMesh();
	par_shapes_free_mesh(par_mesh);

}

void Component_Mesh::GenerateImported(aiMesh* new_mesh, aiNode* node)
{
	num_vertex = new_mesh->mNumVertices;
	vertex = new float[num_vertex * 3];
	memcpy(vertex, new_mesh->mVertices, sizeof(float) * num_vertex * 3);
	LOG("New FBX mesh with %d vertices", num_vertex);

	//copy normals
	if (new_mesh->HasNormals())
	{
		has_normals = true;
		num_normal = new_mesh->mNumVertices;
		normal = new float[new_mesh->mNumVertices * 3];
		memcpy(normal, new_mesh->mNormals, sizeof(aiVector3D) * num_vertex);
		LOG("New FBX mesh with %d normals", num_normal);
	}


	//copy uvs
	for (int k = 0; k < new_mesh->GetNumUVChannels(); ++k)
	{
		if (new_mesh->HasTextureCoords(k)) {

			num_uvs = new_mesh->mNumVertices;
			uvs = new float[num_uvs * 2];
			uint j = 0;
			for (uint i = 0; i < new_mesh->mNumVertices; ++i) {
				has_uvs = true;
				//there are two for each vertex
				memcpy(&uvs[j], &new_mesh->mTextureCoords[k][i].x, sizeof(float));
				memcpy(&uvs[j + 1], &new_mesh->mTextureCoords[k][i].y, sizeof(float));
				j += 2;
			}
			LOG("New FBX mesh with %d UVs", num_uvs);
		}
	}
	//copy color
	if (new_mesh->HasVertexColors(0)) //need to put a var
	{
		num_color = new_mesh->mNumVertices;
		color = new float[num_color * 4];
		uint j = 0;
		for (uint i = 0; i < new_mesh->mNumVertices; ++i)
		{
			memcpy(&color[j], &new_mesh->mColors[0][i].r, sizeof(float));
			memcpy(&color[j + 1], &new_mesh->mColors[0][i].g, sizeof(float)); //row var needed
			memcpy(&color[j + 2], &new_mesh->mColors[0][i].b, sizeof(float));
			memcpy(&color[j + 3], &new_mesh->mColors[0][i].a, sizeof(float));
			j += 4;
		}
		LOG("New FBX mesh with %d colors", num_color);
	}



	//copy faces
	if (new_mesh->HasFaces())
	{
		num_index = new_mesh->mNumFaces * 3;
		index = new uint[num_index]; // assume each face is a triangle

		for (uint j = 0; j < new_mesh->mNumFaces; ++j)
		{
			if (new_mesh->mFaces[j].mNumIndices != 3) {
				LOG("WARNING, geometry face with != 3 indices!");
				test = true;
			}
			else {
				memcpy(&index[j * 3], new_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
			}
		}
		LOG("New FBX mesh with %d index", num_index);
	}


	aiVector3D translation, scaling;
	aiQuaternion rotation;
	node->mTransformation.Decompose(scaling, rotation, translation);

	//Get the max value of the three components of the "scaling"
	int max_ = max(scaling.x, scaling.y);
	max_ = max(max_, scaling.z);

	float3 pos(translation.x, translation.y, translation.z);
	// Divide the scaling by it's max number to set to 1 the scale factor, and keeping the relation
	float3 scale(scaling.x / max_, scaling.y / max_, scaling.z / max_); 
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

	gameObject_Item->AddComponent(GO_COMPONENT::TRANSFORM, pos, scale, rot);
	

	gl_Int = true;

	if (!test)
		CreateFaceNormals();

	CreateMesh();
}

void Component_Mesh::GenerateCone()
{
	par_mesh = par_shapes_create_cone(10,10);
	par_shapes_translate(par_mesh, 0.0f, 0.0f, 0.0f);
	par_shapes_unweld(par_mesh, true);
	par_shapes_compute_normals(par_mesh);

	num_vertex = par_mesh->npoints;
	num_index = par_mesh->ntriangles * 3;

	vertex = new float[num_vertex * 3];
	index = new uint[num_index * 3];

	memcpy(vertex, par_mesh->points, sizeof(float) * num_vertex * 3);
	memcpy(index, par_mesh->triangles, sizeof(PAR_SHAPES_T) * num_index);

	num_normal = par_mesh->npoints;
	normal = new float[num_vertex * 3];
	memcpy(normal, par_mesh->normals, sizeof(float) * num_vertex * 3);


	num_uvs = par_mesh->npoints;
	uvs = new float[num_uvs * 2];

	gl_Short = true;
	has_normals = true;
	has_uvs = true;

	LOG("New Cone mesh with %d vertices", num_vertex);
	LOG("New Cone mesh with %d index", num_index);
	LOG("New Cone mesh with %d normals", num_normal);
	LOG("New Cone mesh with %d UVs", num_uvs);

	CreateVertexFaceNormals();
	CreateMesh();
	par_shapes_free_mesh(par_mesh);

}

void Component_Mesh::GenerateCylinder()
{
	par_mesh = par_shapes_create_cylinder(100,10);
	par_shapes_translate(par_mesh, 0.0f, 0.0f, 0.0f);
	par_shapes_unweld(par_mesh, true);
	par_shapes_compute_normals(par_mesh);

	num_vertex = par_mesh->npoints;
	num_index = par_mesh->ntriangles * 3;

	vertex = new float[num_vertex * 3];
	index = new uint[num_index * 3];

	memcpy(vertex, par_mesh->points, sizeof(float) * num_vertex * 3);
	memcpy(index, par_mesh->triangles, sizeof(PAR_SHAPES_T) * num_index);

	num_normal = par_mesh->npoints;
	normal = new float[num_vertex * 3];
	memcpy(normal, par_mesh->normals, sizeof(float) * num_vertex * 3);


	num_uvs = par_mesh->npoints;
	uvs = new float[num_uvs * 2];

	gl_Short = true;
	has_normals = true;
	has_uvs = true;

	LOG("New Cylinder mesh with %d vertices", num_vertex);
	LOG("New Cylinder mesh with %d index", num_index);
	LOG("New Cylinder mesh with %d normals", num_normal);
	LOG("New Cylinder mesh with %d UVs", num_uvs);

	CreateVertexFaceNormals();
	CreateMesh();
	par_shapes_free_mesh(par_mesh);

}

void Component_Mesh::GenerateDodecahedron()
{
	par_mesh = par_shapes_create_dodecahedron();
	par_shapes_translate(par_mesh, 0.0f, 0.0f, 0.0f);
	par_shapes_unweld(par_mesh, true);
	par_shapes_compute_normals(par_mesh);

	num_vertex = par_mesh->npoints;
	num_index = par_mesh->ntriangles * 3;

	vertex = new float[num_vertex * 3];
	index = new uint[num_index * 3];

	memcpy(vertex, par_mesh->points, sizeof(float) * num_vertex * 3);
	memcpy(index, par_mesh->triangles, sizeof(PAR_SHAPES_T) * num_index);

	num_normal = par_mesh->npoints;
	normal = new float[num_vertex * 3];
	memcpy(normal, par_mesh->normals, sizeof(float) * num_vertex * 3);


	num_uvs = par_mesh->npoints;
	uvs = new float[num_uvs * 2];

	gl_Short = true;
	has_normals = true;
	has_uvs = true;

	LOG("New Dodecahedron mesh with %d vertices", num_vertex);
	LOG("New Dodecahedron mesh with %d index", num_index);
	LOG("New Dodecahedron mesh with %d normals", num_normal);
	LOG("New Dodecahedron mesh with %d UVs", num_uvs);

	CreateVertexFaceNormals();
	CreateMesh();
	par_shapes_free_mesh(par_mesh);

}

void Component_Mesh::GeneratePlane()
{
	par_mesh = par_shapes_create_plane(10,1);
	par_shapes_translate(par_mesh, 0.0f, 0.0f, 0.0f);
	par_shapes_unweld(par_mesh, true);
	par_shapes_compute_normals(par_mesh);

	num_vertex = par_mesh->npoints;
	num_index = par_mesh->ntriangles * 3;

	vertex = new float[num_vertex * 3];
	index = new uint[num_index * 3];

	memcpy(vertex, par_mesh->points, sizeof(float) * num_vertex * 3);
	memcpy(index, par_mesh->triangles, sizeof(PAR_SHAPES_T) * num_index);

	num_normal = par_mesh->npoints;
	normal = new float[num_vertex * 3];
	memcpy(normal, par_mesh->normals, sizeof(float) * num_vertex * 3);


	if (par_mesh->tcoords != nullptr) {
		uvs = new float[num_vertex * 3];
		memcpy(uvs, par_mesh->tcoords, sizeof(float) * num_vertex * 3);
	}

	gl_Short = true;
	has_normals = true;
	has_uvs = true;

	LOG("New Plane mesh with %d vertices", num_vertex);
	LOG("New Plane mesh with %d index", num_index);
	LOG("New Plane mesh with %d normals", num_normal);
	LOG("New Plane mesh with %d UVs", num_uvs);

	CreateVertexFaceNormals();
	CreateMesh();
	par_shapes_free_mesh(par_mesh);

}
void Component_Mesh::GenerateTorus()
{
	par_mesh = par_shapes_create_torus(10, 10,1); //slices & stacks need to be more than 3 to create a torus radius bewteen 0.1 && 1
	//par_shapes_translate(par_mesh, 0.0f, 0.0f, 0.0f);
	par_shapes_unweld(par_mesh, true);
	par_shapes_compute_normals(par_mesh);

	num_vertex = par_mesh->npoints;
	vertex = par_mesh->points;

	num_index = par_mesh->ntriangles;
	index = (uint*)par_mesh->triangles;

	num_normal = par_mesh->npoints;
	normal = par_mesh->normals;

	num_uvs = par_mesh->npoints;
	uvs = par_mesh->tcoords;

	gl_Short = true;
	has_normals = true;
	has_uvs = true;

	LOG("New Torus mesh with %d vertices", num_vertex);
	LOG("New Torus mesh with %d index", num_index);
	LOG("New Torus mesh with %d normals", num_normal);
	LOG("New Torus mesh with %d UVs", num_uvs);

	CreateVertexFaceNormals();
	CreateMesh();
	par_shapes_free_mesh(par_mesh);

}


void Component_Mesh::CreateMesh()
{
	//Cube Vertex
	id_vertex = 0;
	glGenBuffers(1, (GLuint*) &(id_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertex * 3, vertex, GL_STATIC_DRAW);


	//Cube Vertex definition
	id_index = 0;
	glGenBuffers(1, (GLuint*) &(id_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	if (type == OBJECT_TYPE::IMPORTER)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_index, index, GL_STATIC_DRAW);
	}

	else
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_index * 3, index, GL_STATIC_DRAW);
	}
	//IndexNormal
	id_normal = 0;
	glGenBuffers(1, (GLuint*)&id_normal);
	glBindBuffer(GL_ARRAY_BUFFER, id_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_normal * 3, normal, GL_STATIC_DRAW);

	//UVs
	id_uvs = 0;
	glGenBuffers(1, (GLuint*) &id_uvs);
	glBindBuffer(GL_ARRAY_BUFFER, id_uvs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_uvs * 2, uvs, GL_STATIC_DRAW);

	
}

void Component_Mesh::CreateFaceNormals()
{
	debug_mesh debItem;
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

void Component_Mesh::CreateVertexFaceNormals()
{
	debug_mesh debItem;
	for (int i = 0; i < num_index *3; i++)
	{
		//Triangle points using indices and vertices
		uint index_01 = i *3;
		uint index_02 = (i + 1) *3;
		uint index_03 = (i + 2) *3;

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

void Component_Mesh::TranslateMesh(float pos[3])
{
	//THE PARENT DON'T HAVE A MESH NORMALLY, SO THIS WILL CRASH BECAUSE THERE'S NO MESH ON IT
	gameObject_Item->game_object_pos.x = pos[0];
	gameObject_Item->game_object_pos.y = pos[1];
	gameObject_Item->game_object_pos.z = pos[2];

	//par_shapes_translate(par_mesh, gameObject_Item->game_object_pos.x, gameObject_Item->game_object_pos.y, gameObject_Item->game_object_pos.z);
}

std::list<debug_mesh> Component_Mesh::GetDebugInfo()
{
	return mesh_debug;
}


