#include "Application.h"
#include "Cube.h"
#include "ModuleUI.h"
#include "par_shapes.h"

void Cube::CreateMesh()
{
	//Cube Vertex
	my_id = 0;
	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*cube_mesh->npoints * 3, cube_mesh->points, GL_STATIC_DRAW);

	//Cube Vertex definition
	my_indices = 0;
	glGenBuffers(1, (GLuint*) &(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*cube_mesh->ntriangles * 3, cube_mesh->triangles, GL_STATIC_DRAW);

	//IndexNormal
	glGenBuffers(1, &id_normal);
	glBindBuffer(GL_ARRAY_BUFFER, id_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cube_mesh->npoints * 3, cube_mesh->normals, GL_STATIC_DRAW);
}

void Cube::Render()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, cube_mesh->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);



	//draw vertex
	if (App->ui->show_vertex)
	{
		glPointSize(2.5f);
		glBegin(GL_POINTS);
		glColor4fv((float*)& App->importer->vertex_color);
		for (int i = 0; i < cube_mesh->npoints * 3; i += 3)
		{
			glVertex3f(cube_mesh->points[i], cube_mesh->points[i + 1], cube_mesh->points[i + 2]);
		}

		glColor3f(255.0f, 255.0f, 255.0f);
		glEnd();
	}

	//draw normals
	if (App->ui->show_normals)
	{
		glLineWidth(2.5f);
		glColor4fv((float*)& App->importer->vertex_normal_color);

		
		memcpy(cube_mesh->normals, cube_mesh->normals, sizeof(float) * cube_mesh->npoints * 3);

		for (int i = 0; i < cube_mesh->npoints * 3; i += 3)
		{
			glBegin(GL_LINES);
			
			vec3 normalVec = normalize({ cube_mesh->normals[i],cube_mesh->normals[i+1], cube_mesh->normals[i+2] });
			glVertex3f(cube_mesh->points[i], cube_mesh->points[i + 1], cube_mesh->points[i + 2]);
			glVertex3f(cube_mesh->points[i] + normalVec.x, cube_mesh->points[i + 1] + normalVec.y, cube_mesh->points[i + 2] + normalVec.z);
	
			glEnd();
		}
	}

}
