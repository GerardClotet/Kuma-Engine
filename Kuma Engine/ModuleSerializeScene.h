#pragma once


#include "Globals.h"
#include "Application.h"
#include "Module.h"

#include <unordered_map> 
#include <vector>

class GameObject;
class SerializerR_JSON;

enum class GO_Properties
{
	NONE = -1,
	UUID, //0 
	PARENT_UUID,
	NAME,
	COMPONENT,
	POSITION,
	ROTATION,
	SCALE,
	META_ROUTE,
	TEXTURE_PATH,
	FRUSTRUM_TYPE,
	ASPECT_RATIO,
	NEAR_PLANE,
	FAR_PLANE,
	HORIZONTAL_FOV,
	VERTICAL_FOV,
	FRUSTRUM_COLOR,
	CAMERA_BACKGROUND_COLOR,
	FRONT,
	UP
};


struct infoToFill {
	std::string UUID,
	parent_UUID,GO_name;
	GameObject* go_to_fill;
	void ChooseWhatToFill(const char* field,void* u );
	void Go_ElementToFill(const char* field);
	int _counter = 0;
	GO_Properties go_p;

	float3 t_pos; //used also for component Camera Position (frustum)
	Quat t_rot;
	float3 t_scale;

	float3 front_c;
	float3 up_c;
	

	const char* names_r_json[19]
	{
		"UUID", //0
		"Parent UUID",
		"Name",
		//"Components",// this may not be used
		"Component",
		"Position",
		"Rotation",
		"Scale",
		"Meta Route",
		"Texture Path",
		"FrustumType",
		"AspectRatio",
		"Near Plane",
		"Far Plane",
		"Horizontal Fov",
		"Vertical Fov",
		"Frustum Color",
		"Camera Background Color",
		"front",
		"Up", //17

	};
};
struct SceneObjects {
	std::vector<GameObject*> scene_go_vec;
	infoToFill* inf_to_f;
	void FamilyGameObjects(std::vector<GameObject*> go);
	
};

class ModuleSerializeScene : public Module
{
public:
	ModuleSerializeScene(Application* app, bool start_enabled = true);
	~ModuleSerializeScene();

	update_status Update(float dt);

	void LoadScene(const char* path);
	void SaveScene(const char* path);

	SerializerR_JSON* json_serializer;

	int selectR_jsonType(rapidjson::Type type);
	void parseRecursive(std::string scope, rapidjson::Value::ConstMemberIterator object/*, std::unordered_map<std::string, std::string>& values*/);
private:
	std::string current_scene;
	std::unordered_map<std::string , std::string > _unorder;

	SceneObjects* sc_obj;
};