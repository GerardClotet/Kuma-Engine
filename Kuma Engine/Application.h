#pragma once

#include "glew-2.1.0/include/GL/glew.h"
#include <list>
#include <vector>
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleUI.h"
#include "ModuleHardware.h"
#include "Parson/parson.h"

#include "PerfTimer.h"
#include "Timer.h"
class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* ui;
	ModuleHardware* hardware;
private:

	Timer		ms_timer;
	float		dt;
	uint64		frame_count =0;
	uint32		last_sec_frame_count;
	uint32		prev_last_sec_frame_count;
	bool		pause = false;
	PerfTimer	ptimer;
	Timer		last_sec_frame_time;
	Timer		frame_time;
	int			framerate_cap = 60;

	JSON_Object* config = nullptr;

	std::string app_name;
	std::string org_name;
	std::string version;
	std::string license;

public:
	std::list<Module*> list_modules;
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	float GetDt();
	uint GetFramerateCap();
	void SetFramerateCap(uint cap);
	uint GetFramesOnLatsUpdate();
	bool LoadConfigAllModules();
	bool SaveConfigAllModules();
	void LoadConfig(JSON_Object*& config);
	void SaveConfig(JSON_Object*& config, std::string path);
	void OpenWebsite(const std::string &link);
	JSON_Object* LoadJSONFile(const std::string &path);
	void Log(const char* fmt,...);
	void saveLog(const char* fmt,...);

	const char* GetAppName() const;
	void SetAppName(const char* name);
	const char* GetOrganizationName() const;
	void SetOrgName(const char* name);
	void SetAppVersion(const char* version);
	const char* GetAppVersion();
	void SetLicense(const char* license);
	const char* GetLicense();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

public:
	bool close_app = false;
	bool vsync = true; //need to add the condition to true

	std::list<const char*> log_saves;
	JSON_Value* value;
};

extern Application* App;