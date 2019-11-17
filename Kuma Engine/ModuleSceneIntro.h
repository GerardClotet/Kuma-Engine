#pragma once
#include "Module.h"
#include "Globals.h"
#include "par_shapes.h"
#include "glmath.h"
#include "Color.h"
#include <list>
#include <vector>
#include <string>
#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLibFwd.h"
#include "RayCast.h"

#define MAX_SNAKE 2



class GameObject;
class Components;
class Component_Camera;
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

	void LoadConfig(JSON_Object*& config);
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void createGrid();
	void createDirtyCube();
	GameObject* CreateGameObject(GameObject* parent,OBJECT_TYPE type, std::string name);
	void UpdateGameObject(GameObject* parent);
	GameObject* MyRayCastIntersection(LineSegment* ray, RayCast& hit);
	void BoxIntersection(GameObject* obj, LineSegment* ray, std::vector<RayCast>& scene_obj);
	bool TriangleTest(LineSegment* ray, std::vector<RayCast>& scene_obj, RayCast& point);
	


	GameObject* root = nullptr; //emptygame objects containing all game obects
	GameObject* selected_game_obj = nullptr;
	Component_Camera* camera_hardcoded = nullptr;
	std::vector<Components*> camera_list;

public:
	//std::list<Spherestruct*> sphere_struct_list;
	SDL_Window* window;
	uint my_id;
	uint indiceS = 0;
	//std::vector<GameObject*> gameObject_list; //not using anyumoe
private:
	int max_grid = 75;
	float sunX = 2;
	float sunZ = 150;
	float sunQ = 2;
	float sunW = 150;
	int separator = 2;
	std::string firstFbx;
	std::string firstTex;

};
