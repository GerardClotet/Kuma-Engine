#include "Application.h"
#include "Mesh.h"
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
	glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}