#pragma once
#include "Module.h"


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
class PanelFile;


class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();
	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	static void HelpMarker(const char* desc);
	void LoadConfig(JSON_Object*& config);
	void LoadInputEvent(uint id, uint state);
	void BackgroundDockSpace();
	void ObjectEditor();
	bool show_another_window = false;
	bool demoWindow = false;
	bool stylewindow = false;
	bool show_obj_edit_window = false;
	bool configuration_window = false;
	bool console_window = false;
	bool file_window = false;
	bool file_save_window = false;
	bool config_default = false;
	bool about_window = false;
	bool inspector_window = false;
	bool hierarchy_window = false;


	void AddFPS(float fps, float ms);

	void Log(const char* fmt, ...);
	void LoadFile(const char* filter_extension = nullptr, const char* from_dir = nullptr);
	void SaveFile(const char* filter_extension = nullptr, const char* from_dir = nullptr);

	std::list<Sphere> spherelist;


	std::vector<float> fps_log;
	std::vector<float> ms_log;
	const char* GetExtension(const char* extension,const char* directory = nullptr);
	const char* ChooseExtension(const char* ext);

private:
	//void DisplayConsole();
	//void DisplayConfig();
	void HelpScreen();
	void ViewScreen();
	void FileScreen();
	void GameObjectScreen();
	void ComponentsScreen();
	void DrawDirectoryTree(const char* directory, const char* filter_extension);
	void ShowErrorPopUp(const char* text);

private:
	bool activeWindow = true;
	bool changeColor = false;

	bool alternate_extension = false;
	bool show_error_popUp = false;
	std::string error_text;
	/*std::vector<float>fps_log;
	std::vector<float>ms_log;*/
public:

	PanelConsole* console_p = nullptr;
	PanelConfig* config_p = nullptr;
	PanelAbout* about_p = nullptr;
	PanelInspector* inspector_p = nullptr;
	PanelHierarchy* hierarchy_p = nullptr;
	PanelFile* file_p = nullptr;
	char selected_file[FILE_MAX];
	std::list<Panel*> panel_list;
};