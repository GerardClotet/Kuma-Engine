
#pragma once
#include "Module.h"
//#include "Globals.h"

#include "ImGui/imgui.h"
#include "ImGui/imconfig.h"
#include "ImGui/examples/imgui_impl_opengl3.h"
#include "ImGui/examples/imgui_impl_sdl.h"
#include "ModuleSceneIntro.h"
#include <list>
#include <vector>
#define LOG_ARRAY 100
class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	static void HelpMarker(const char* desc);

	void ObjectEditor();
	bool show_another_window	= false;
	bool demoWindow 			= false;
	bool stylewindow 			= false;
	bool show_obj_edit_window	= false;
	bool configuration_window	= false;
	bool console_window			= false;
	bool config_default			= false;

	void AddFPS(float fps, float ms);
	std::list<Sphere> spherelist;

private:
	void DisplayConsole();
	void DisplayConfig();
	void HelpScreen();
	void ViewScreen();
	void FileScreen();
	void DisplayConfigDefault();

private:
	bool activeWindow = true;
	bool changeColor = false;
	std::vector<float>fps_log;
	std::vector<float>ms_log;
};