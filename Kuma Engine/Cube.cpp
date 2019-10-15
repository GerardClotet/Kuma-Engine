#include "Application.h"
#include "Cube.h"
#include "ModuleUI.h"
#include "ModuleImporter.h"


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



	////draw vertex
	//if (App->ui->show_vertex)
	//{
	//	glPointSize(2.5f);
	//	glBegin(GL_POINTS);
	//	glColor4fv((float*)& App->importer->vertex_color);
	//	for (int i = 0; i < cube_mesh->npoints * 3; i += 3)
	//	{
	//		glVertex3f(cube_mesh->points[i], cube_mesh->points[i + 1], cube_mesh->points[i + 2]);
	//	}

	//	glColor3f(255.0f, 255.0f, 255.0f);
	//	glEnd();
	//}

	////draw normals
	//if (App->ui->show_normals)
	//{
	//	glLineWidth(2.5f);
	//	glColor4fv((float*)& App->importer->vertex_normal_color);

	//	
	//	memcpy(cube_mesh->normals, cube_mesh->normals, sizeof(float) * cube_mesh->npoints * 3);

	//	for (int i = 0; i < cube_mesh->npoints * 3; i += 3)
	//	{
	//		glBegin(GL_LINES);
	//		
	//		vec3 normalVec = normalize({ cube_mesh->normals[i],cube_mesh->normals[i+1], cube_mesh->normals[i+2] });
	//		glVertex3f(cube_mesh->points[i], cube_mesh->points[i + 1], cube_mesh->points[i + 2]);
	//		glVertex3f(cube_mesh->points[i] + normalVec.x, cube_mesh->points[i + 1] + normalVec.y, cube_mesh->points[i + 2] + normalVec.z);
	//
	//		glEnd();
	//	}
	//}

	////draw face normal
	//if (App->ui->show_face_normal)
	//{
	//	CreateFaceNormals();
	//}

}

void Cube::CreateFaceNormals()
{
		glPointSize(2.5f);
		glBegin(GL_LINES);

		glColor4fv((float*)& App->importer->face_normal_color);

		for (int i = 0; i < cube_mesh->ntriangles*3; i++)
		{
			//Triangle points using indices and vertices
			uint index_01 = cube_mesh->triangles[i] * 3;
			uint index_02 = cube_mesh->triangles[i + 1] * 3;
			uint index_03 = cube_mesh->triangles[i + 2] * 3;

			//Calculate the points of the triangle by using the vertex array and indices to find the exact vertex
			float3 p1 = { cube_mesh->points[index_01], cube_mesh->points[index_01 + 1], cube_mesh->points[index_01 + 2] };
			float3 p2 = { cube_mesh->points[index_02], cube_mesh->points[index_02 + 1], cube_mesh->points[index_02 + 2] };
			float3 p3 = { cube_mesh->points[index_03], cube_mesh->points[index_03 + 1], cube_mesh->points[index_03 + 2] };

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


			glVertex3f(C1, C2, C3);
			glVertex3f(C1 + normalizedVec.x, C2 + normalizedVec.y, C3 + normalizedVec.z);

			i += 2;
		}
		glEnd();
}
