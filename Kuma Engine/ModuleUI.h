
#pragma once
#include "Module.h"
//#include "Globals.h"

#include "ImGui/imgui.h"
#include "ImGui/examples/imgui_impl_opengl3.h"
#include "ImGui/examples/imgui_impl_sdl.h"


class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	static void HelpMarker(const char* desc);
	bool show_another_window = false;
	bool demoWindow = false;
	bool stylewindow = false;
};