#pragma once
#include "Module.h"
#include "Globals.h"
#include "par_shapes.h"
#include "glmath.h"
#include "Color.h"
#include <list>
#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLibFwd.h"

#define MAX_SNAKE 2

//struct Spherestruct {
//	par_shapes_mesh* sphere;
//	uint id =0;
//	uint indice=0;
//};

//struct Meshes //It stores all mesh data
//{
//	//vertex
//	uint id_vertex = 0;
//	uint num_vertex = 0;
//	float* vertex = nullptr;
//
//	//indices
//	uint id_index = 0;
//	uint num_index = 0;
//	PAR_SHAPES_T* index = nullptr;
//};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Init();
	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void createCube(const vec3 &position, Color color = { 255,255,255,255 });
	void createGrid();
	void createSphere(const vec3& position, int subdivisions,  Color color = { 255,255,255,255 });
public:
	//std::list<Spherestruct*> sphere_struct_list;
	SDL_Window* window;
	uint ids = 0;
	uint indiceS = 0;
private:
	int max_grid = 75;
	float sunX = 2;
	float sunZ = 150;
	float sunQ = 2;
	float sunW = 150;
	int separator = 2;
	bool firstBuffer = true;

	uint my_idk = 0;
	uint my_indicesk = 0;

	

	
	
};
