#include "Component_Mesh.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "ModuleUI.h"
#include "ModuleTexture.h"
#include "Cube.h"


Component_Mesh::Component_Mesh(OBJECT_TYPE type) : Components()
{
	comp_type = GO_COMPONENT::MESH;
	switch (type)
	{
	case OBJECT_TYPE::NONE:
		break;
	case OBJECT_TYPE::CUBE:
		GenerateCube();
		break;
	case OBJECT_TYPE::SPHERE:
		GenerateSphere();
		break;
	case OBJECT_TYPE::CYLINDRE:
		break;
	case OBJECT_TYPE::CAPSULE:
		break;
	case OBJECT_TYPE::PLANE:
		break;
	case OBJECT_TYPE::IMPORTER:
		//gl_int=true;
		break;
	default:
		break;
	}
}

Component_Mesh::~Component_Mesh()
{

}
bool Component_Mesh::Update()
{
	////Read buffers and draw the shapes
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	////glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	////glEnable(GL_TEXTURE_2D);

	//glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);

	////Read and Draw normals buffers
	//if (has_normals)
	//{
	//	glBindBuffer(GL_ARRAY_BUFFER, id_normal);
	//	glNormalPointer(GL_FLOAT, 0, NULL);

	//}


	////Read and Draw UVS buffers
	//if (has_uvs)
	//{

	//	if (texture != nullptr)
	//		glBindTexture(GL_TEXTURE_2D, texture->id);

	//	//glGenerateMipmap(GL_TEXTURE_COORD_ARRAY);
	//	glBindBuffer(GL_ARRAY_BUFFER, id_uvs);
	//	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	//}


	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);

	//if (gl_Int)
	//	glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_INT, NULL);

	//else if (gl_Short)
	//	glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_SHORT, NULL);


	////	glDisableClientState(GL_TEXTURE_2D);

	//	//glBindTexture(GL_TEXTURE_2D, 0);
	////glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);
	//glDisableClientState(GL_VERTEX_ARRAY);

	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);



	glBindBuffer(GL_ARRAY_BUFFER, id_normal);
	glNormalPointer(GL_FLOAT, 0, NULL);



	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glDrawElements(GL_TRIANGLES, num_index*3, GL_UNSIGNED_SHORT, NULL);
	glDisableClientState(GL_NORMAL_ARRAY);

	glDisableClientState(GL_VERTEX_ARRAY);


	//draw normals
	if (App->ui->show_normals)
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
	if (App->ui->show_vertex)
	{
		glPointSize(2.5f);
		glBegin(GL_POINTS);
		glColor4fv((float*)& App->importer->vertex_color);
		for (int i = 0; i < num_vertex * 3; i += 3)
		{
			glVertex3f(vertex[i], vertex[i + 1], vertex[i + 2]);
		}

		glColor3f(255.0f, 255.0f, 255.0f);
		glEnd();
	}

	//draw face normal
	if (App->ui->show_face_normal)
	{
		std::list<debug_mesh> mesh_debug = App->importer->GetDebugInfo();
		std::list<debug_mesh>::iterator deb = mesh_debug.begin();

		while (deb != mesh_debug.end())
		{
			//Check if face normals is activated
			if (App->ui->show_face_normal) {
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
	vertex = par_mesh->points;

	num_index = par_mesh->ntriangles;
	index = (uint*)par_mesh->triangles;

	num_normal = par_mesh->npoints;
	normal = par_mesh->normals;

	num_uvs = par_mesh->npoints;
	uvs = new float[num_uvs * 2];

	gl_Short = true;
	has_normals = true;
	CreateMesh();
}

void Component_Mesh::GenerateSphere()
{
	par_mesh = par_shapes_create_subdivided_sphere(3);
	par_shapes_translate(par_mesh, 1.0f, 1.0f, 1.0f);
	par_shapes_unweld(par_mesh, true);
	par_shapes_compute_normals(par_mesh);

	num_vertex = par_mesh->npoints;
	vertex = par_mesh->points;

	num_index = par_mesh->ntriangles;
	index = (uint*)par_mesh->triangles;

	num_normal = par_mesh->npoints;
	normal = par_mesh->normals;

	num_uvs = par_mesh->npoints;
	uvs = new float[num_uvs * 2];

	gl_Short = true;
	has_normals = true;
	CreateMesh();
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*num_index * 3, index, GL_STATIC_DRAW);

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

	LOG("Created mesh with vertex id: %i , index id: %i, normal id: %i  and uvs id: %i", id_vertex, id_index, id_normal, id_uvs);
}

