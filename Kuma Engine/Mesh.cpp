#include "Application.h"
#include "Mesh.h"
#include "ModuleUI.h"
#include "par_shapes.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

void Mesh::CreateMesh()
{
	//Vertex buffer
	glGenBuffers(1, &id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* num_vertex * 3, vertex, GL_STATIC_DRAW);

	//Index buffer
	glGenBuffers(1, &id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*num_index, index, GL_STATIC_DRAW);

	LOG("Created mesh with vertex id: %i and index id: %i", id_vertex, id_index);

}

void Mesh::Render()
{
	//Read buffers and draw the shapes

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_INT, index);
	glDisableClientState(GL_VERTEX_ARRAY);

	//draw normals
	if (App->ui->show_normals)
	{
		glBegin(GL_LINES);
		
		for (int i = 0; i < num_normal * 3; i+=3)
		{
			glVertex3f(vertex[i], vertex[i + 1], vertex[i + 2]);
			glVertex3f(vertex[i] + normal[i], vertex[i + 1] + normal[i + 1], vertex[i + 2] + normal[i + 2]);

		}

		glEnd();
	}

	//draw vertex
	if (App->ui->show_vertex)
	{
		glBegin(GL_POINTS);

		for (int i = 0; i < num_vertex * 3; i += 3)
		{
			glVertex3f(vertex[i], vertex[i + 1], vertex[i + 2]);
		}		


		glEnd();
	}

}

void Mesh::createCube(const vec3 & position, Color color)
{
	FBX* aux_fbx = new FBX();
	Mesh* aux_mesh = new Mesh();
	par_shapes_mesh* cube;
	cube = par_shapes_create_cube();

	aux_mesh->num_index = cube->ntriangles;
	aux_mesh->index = (uint*)cube->triangles;
	aux_mesh->num_vertex = cube->npoints;
	aux_mesh->vertex = cube->points;

	par_shapes_translate(cube, position.x, position.y, position.z);

	// buffer points
	glGenBuffers(1, &id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertex * 3, vertex, GL_STATIC_DRAW);

	// buffer index
	glGenBuffers(1, &id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * num_index * 3, index, GL_STATIC_DRAW);

	aux_fbx->mesh_list_fbx.push_back(aux_mesh);
}
