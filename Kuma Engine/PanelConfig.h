#ifndef PANEL_CONFIG_H_
#define PANEL_CONFIG_H_


#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "Panel.h"
#include "Application.h"
#include <vector>
#include <windows.h>
#include "Module.h"
#include "ModuleWindow.h"
#include "ImGui/imgui.h"


class PanelConfig : public Panel
{
public:
	PanelConfig() {};
	PanelConfig(const char* name) :Panel(name) {
		App->saveLog("Config Panel Created");
	};
	~PanelConfig() {};

	update_status Draw();
	void DisplayConfig();
	void AddInputToBuffer(const char* input);
	bool Getwireframe();
	bool GetFill();

private:
	bool configuration_window = false;
	bool open = false;
	//std::vector<float> fps_log;
	//std::vector<float> ms_log;
	bool activeWindow		= true;
	bool activeFileSystem	= true;
	bool activeInput		= true;
	bool config_default		= false;
	char* basePath			= nullptr;
	std::string orgname		= "UPC CITM";
	ImGuiTextBuffer	input_buf;
	bool scrollToBottom		= false;


	uint w = App->window->GetScreenWidth();
	uint h = App->window->GetScreenHeight();

private:
	std::string SelectFile(); //This allows you to select a .json file
	char filename[MAX_PATH]; //This is the variable that will recieve the file path
	OPENFILENAME ofn; //struct with all the info needed to create the dialogue to select the file
	bool GL_Depth_test = true;
	bool GL_Cull_face = true;
	bool GL_Lightning = true;
	bool GL_Color_material = true;

	bool GL_Wireframe = true;
	bool GL_Fill = true;
};



#endif // !PANEL_CONFIG_H_
