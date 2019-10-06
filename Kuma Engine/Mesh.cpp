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

	glGenBuffers(1, &id_uvs);
	glBindBuffer(GL_ARRAY_BUFFER, id_uvs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_uvs * 2, uvs, GL_STATIC_DRAW);
	LOG("Created mesh with vertex id: %i , index id: %i, normal id: %i  and uvs id: %i", id_vertex, id_index, id_normal,id_uvs);
}

void Mesh::Render()
{
	//Read buffers and draw the shapes
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	//Read and Draw normals buffers
	if (has_normals)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, id_normal);
		glNormalPointer(GL_FLOAT, 3, NULL);
	}


	//Read and Draw UVS buffers
	if (has_uvs)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, id_uvs);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	}
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_INT, NULL);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);


	




	//draw normals
	if (App->ui->show_normals)
	{
		glLineWidth(2.5f);
		glBegin(GL_LINES);

		glColor3f(0.0f, 255.0f, 255.0f);

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
		glColor3f(255.0f, 0.0f, 0.0f);
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

				glColor3f(255.0f, 0.0f, 255.0f); 

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
