#pragma once
#include "Module.h"
#include "Globals.h"
#include "par_shapes.h"
#include "glmath.h"
#include "Color.h"
#include "Mesh.h"
#include <list>
#include <vector>
#include <string>
#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLibFwd.h"

#define MAX_SNAKE 2
#define CHECKERS_WIDTH 100	
#define CHECKERS_HEIGHT 100


class GameObject;
struct SDL_Window;
struct aiMesh;
enum class OBJECT_TYPE;

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
	void createGrid();
	void createDirtyCube();

	//AddGameObject
	//GameObject* AddGameObject(std::string name, OBJECT_TYPE type); // not using
	//GameObject* AddGameObject(std::string name, OBJECT_TYPE type, aiMesh* mesh); //not using anymore
	//
	
	GameObject* CreateGameObject(GameObject* parent,OBJECT_TYPE type,std::string name);
	void UpdateGameObject(GameObject* parent);

	GameObject* root = nullptr; //emptygame objects containing all game obects

public:
	//std::list<Spherestruct*> sphere_struct_list;
	SDL_Window* window;
	uint my_id;
	byte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];;
	uint indiceS = 0;
	//std::vector<GameObject*> gameObject_list; //not using anyumoe
	uint ImageName;
private:
	int max_grid = 75;
	float sunX = 2;
	float sunZ = 150;
	float sunQ = 2;
	float sunW = 150;
	int separator = 2;

};
