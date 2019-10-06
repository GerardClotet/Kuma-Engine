#include "Application.h"
#include "Mesh.h"
#include "ModuleUI.h"
#include "ModuleSceneIntro.h"
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

	//IndexNormal
	glGenBuffers(1, &id_normal);
	glBindBuffer(GL_ARRAY_BUFFER, id_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_normal * 3, normal, GL_STATIC_DRAW);

	LOG("Created mesh with vertex id: %i , index id: %i and normal id: %i ", id_vertex, id_index, id_normal);
}

void Mesh::Render()
{
	//Read buffers and draw the shapes
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);


	//Read and Draw normals buffers
	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_normal);
	glNormalPointer(GL_FLOAT, 3, NULL);
	glDisableClientState(GL_NORMAL_ARRAY);

	glUnlockArraysEXT();



	//draw normals
	if (App->ui->show_normals)
	{

		glBegin(GL_LINES);

		for (int i = 0; i < num_normal * 3; i += 3)
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

	//draw face normal
	if (App->ui->show_face_normal)
	{
		std::list<geo_debug> m_debug = App->importer->GetDebugInfo();
		std::list<geo_debug>::const_iterator deb = m_debug.begin();

		while (deb != m_debug.end())
		{
			//Face normals
			if (App->ui->show_face_normal) {

				glBegin(GL_LINES);

				glColor3f(255.0f, 0.0f, 255.0f); //Red

				for (int i = 0; i < deb->tri_normal.size(); i++) {

					glVertex3f(deb->tri_center[i].x, deb->tri_center[i].y, deb->tri_center[i].z);
					glVertex3f(deb->tri_center[i].x + deb->tri_normal[i].x, deb->tri_center[i].y + deb->tri_normal[i].y,
						deb->tri_center[i].z + deb->tri_normal[i].z);

				}
				glColor3f(255.0f, 255.0f, 255.0f); //White

				glEnd();
			}

			deb++;
		}

	}

}

void Mesh::createCube(const vec3 & position, Color color)
{
	glColor3f(color.r, color.g, color.b);
	Mesh* mesh = new Mesh();
	par_shapes_mesh* cube;
	cube = par_shapes_create_cube();

	mesh->num_index = cube->ntriangles;
	mesh->index = (uint*)cube->triangles;
	mesh->num_vertex = cube->npoints;
	mesh->vertex = cube->points;

	par_shapes_translate(cube, position.x, position.y, position.z);

	// buffer points
	glGenBuffers(1, &mesh->id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertex * 3, mesh->vertex, GL_STATIC_DRAW);

	// buffer index
	glGenBuffers(1, &mesh->id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh->num_index * 3, mesh->index, GL_STATIC_DRAW);

	App->scene_intro->mesh_list.push_back(mesh);
}
