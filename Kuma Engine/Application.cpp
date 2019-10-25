#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleHardware.h"
#include "ModuleImporter.h"
#include "ModuleTexture.h"

#include "Parson/parson.h"
#include "p2Defs.h"
#include "ModuleUI.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

Application::Application()
{
	PERF_START(ptimer);
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	ui = new ModuleEditor(this);
	hardware = new ModuleHardware(this);
	importer = new ModuleImporter(this);
	texture = new ModuleTexture(this);
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);

	AddModule(importer);
	AddModule(texture);
	// Scenes
	AddModule(scene_intro);
	AddModule(ui);
	AddModule(hardware);
	// Renderer last!
	AddModule(renderer3D);


	float time = SDL_GetTicks();
	memory.push_back(m_getMemoryStatistics().totalActualMemory);

	PERF_PEEK(ptimer);
}

Application::~Application()
{
	std::list<Module*>::reverse_iterator item;
	item = list_modules.rbegin();

	while (item != list_modules.rend())
	{
		RELEASE(*item);
		--item;
	}

	list_modules.clear();

	//to free memory
	json_value_free(json_object_get_value(config, "Configuration/config.json"));
}

bool Application::Init()
{
	PERF_START(ptimer);
	bool ret = true;

	config = LoadJSONFile("Configuration/config.json");
	if (config != nullptr)
	{
		LoadConfigAllModules();
	}

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init();
		++item;
	}

	// After all Init calls we call Start() in all modules
	LOG("--------------Application Start---------------");
	saveLog("--------------Application Start---------------");
	item = list_modules.begin();
	
	while (item != list_modules.end() && ret == true)
	{
		ret = (*item)->Start();
		
		++item;
	}
	
	ms_timer.Start();



	PERF_START(ptimer);

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;
	if (pause)
		dt = 0.0f;
	else
		dt = 1.0f / framerate_cap;
//	dt = (float)ms_timer.Read() / 1000.0f;
	frame_time.Start();

}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}
	float avg_fps = (float)frame_count / ms_timer.ReadSec();
	float seconds_since_startup = ms_timer.ReadSec();
	double last_frame_ms = frame_time.Read();


	/*static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %.2f Last sec frames: %i Vsync: %s ",
		avg_fps, last_frame_ms, prev_last_sec_frame_count, vsync ? "ON" : "OFF");
	window->SetTitle(title);*/

	if (framerate_cap == 0)
		framerate_cap = 1;
	float waiting_time = (1000 / framerate_cap) - last_frame_ms;
	if (waiting_time > (1000 / framerate_cap))
	{
		waiting_time = (1000 / framerate_cap);
	}
	else if (waiting_time < 0)
	{
		waiting_time = 0;
	}
//	window->SetTitle[256]
	if (vsync)
	{
		SDL_Delay(waiting_time);

	}

	ui->AddFPS(prev_last_sec_frame_count, last_frame_ms);



}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = list_modules.begin();
	
	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		++item;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		++item;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
		++item;
	}

	if (close_app)
		ret = UPDATE_STOP;

	FinishUpdate();

	return ret;
}

bool Application::CleanUp()
{
	PERF_START(ptimer);

	bool ret = true;
	std::list<Module*>::reverse_iterator item;
	item = list_modules.rbegin();

	while (item != list_modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		++item;
	}
	list_modules.clear();

	PERF_PEEK(ptimer);

	return ret;
}

float Application::GetDt()
{
	return dt;
}

uint Application::GetFramerateCap()
{
	return framerate_cap;
}

void Application::SetFramerateCap(uint cap)
{
	framerate_cap = cap;
}

uint Application::GetFramesOnLatsUpdate()
{
	return prev_last_sec_frame_count;
}

bool Application::LoadConfigAllModules()
{
	bool ret = true;
	std::list<Module*>::iterator item = list_modules.begin();
	while (item != list_modules.end())
	{
		(*item)->LoadConfig(config);
		++item;
	}
	LoadConfig(config);
	return ret;
}


void Application::LoadConfig(JSON_Object *& config)
{
	app_name = json_object_dotget_string(config, "Application.Name");
	org_name = json_object_dotget_string(config, "Application.Organization");
	version = json_object_dotget_string(config, "Application.Version");
	std::string firstline = json_object_dotget_string(config, "License.firstLine");
	std::string secondLine = json_object_dotget_string(config, "License.secondLine");
	license = firstline + secondLine;

	SetAppName(app_name.c_str());
	SetOrgName(org_name.c_str());
	SetAppVersion(version.c_str());
	SetLicense(license.c_str());
	LOG("load JSON config");
	App->saveLog("load JSON config");
}

void Application::SaveConfig(JSON_Object *& config, std::string path)
{
	json_object_dotset_string(config, "Application.Name", app_name.c_str());
}

void Application::SaveConfigFinish(std::string path)
{
	json_serialize_to_file_pretty(save_value, path.c_str());
}

bool Application::SaveConfigAllModules()
{
	bool ret = true;
	/*std::list<Module*>::iterator item = list_modules.begin();
	while (item != list_modules.end())
	{
		(*item)->SaveConfig(config);
		++item;
	}*/
	return ret;
}



JSON_Object * Application::LoadJSONFile(const std::string & path)
{
	value = json_parse_file(path.data());
	JSON_Object* object = json_value_get_object(value);

	if (value == nullptr || object == nullptr)
	{
		LOG("Error loading file %s", path);
		saveLog("Error loading file %s", path);
	}

	return object;
}

void Application::Log(const char* fmt,...)
{
	ui->Log(fmt);
}

void Application::saveLog(const char* fmt,...)
{
	log_saves.push_back(fmt);
}

const char * Application::GetAppName() const
{
	return app_name.c_str();
}

void Application::SetAppName(const char * name)
{
	if (name != nullptr)
	{
		app_name = name;
		window->SetTitle(name);
	}
}

const char * Application::GetOrganizationName() const
{
	return org_name.c_str();
}

void Application::SetOrgName(const char * name)
{
	if (name != nullptr)
	{
		org_name = name;
	}
}

void Application::SetAppVersion(const char * version)
{
	if (version != nullptr)
		this->version = version;
}

const char * Application::GetAppVersion()
{
	return version.c_str();
}

void Application::SetLicense(const char * license)
{
	if (license != nullptr)
		this->license = license;
}

const char * Application::GetLicense()
{
	return license.c_str();
}

void Application::OpenWebsite(const std::string & link)
{
	ShellExecuteA(NULL, "open", link.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}