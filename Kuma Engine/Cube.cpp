#include "Application.h"
#include "Cube.h"
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
}

void Cube::Render()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, cube_mesh->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}
