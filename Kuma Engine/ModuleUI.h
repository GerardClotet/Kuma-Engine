
#pragma once
#include "Module.h"
//#include "Globals.h"


#include "ModuleSceneIntro.h"
#include <list>
#include <vector>
#include "Panel.h"

#define LOG_ARRAY 100
class Panel;
class PanelConfig;
class PanelConsole;
class PanelAbout;
class PanelInspector;
class PanelHierarchy;


class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();
	bool Start();
	update_status PostUpdate(float dt);
	bool CleanUp();
	static void HelpMarker(const char* desc);
	void LoadConfig(JSON_Object*& config);
	void LoadInputEvent(uint id, uint state);

	void ObjectEditor();
	bool show_another_window	= false;
	bool demoWindow 			= false;
	bool stylewindow 			= false;
	bool show_obj_edit_window	= false;
	bool configuration_window	= false;
	bool console_window			= false;
	bool config_default			= false;
	bool about_window			= false;
	bool inspector_window		= false;
	bool hierarchy_window		= false;
	bool show_normals			= false;
	bool show_vertex			= false;
	bool show_face_normal		= false;

	void AddFPS(float fps, float ms);
	
	void Log(const char* fmt,...);

	std::list<Sphere> spherelist;


	std::vector<float> fps_log;
	std::vector<float> ms_log;
private:
	//void DisplayConsole();
	//void DisplayConfig();
	void HelpScreen();
	void ViewScreen();
	void FileScreen();
	void GameObjectScreen();
	void ComponentsScreen();

private:
	bool activeWindow = true;
	bool changeColor = false;
	/*std::vector<float>fps_log;
	std::vector<float>ms_log;*/
public:

	PanelConsole* console_p		= nullptr;
	PanelConfig* config_p		= nullptr;
	PanelAbout* about_p			= nullptr;
	PanelInspector* inspector_p = nullptr;
	PanelHierarchy* hierarchy_p = nullptr;

	std::list<Panel*> panel_list;
};