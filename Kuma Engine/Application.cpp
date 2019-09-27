#include "Application.h"
#include "Parson/parson.h"
#include "p2Defs.h"
#include "mmgr/mmgr.h"


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
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);

	
	// Scenes
	AddModule(scene_intro);
	AddModule(ui);
	AddModule(hardware);
	// Renderer last!
	AddModule(renderer3D);

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
	json_value_free(json_object_get_value(config, "config.json"));
}

bool Application::Init()
{
	PERF_START(ptimer);
	bool ret = true;

	config = LoadJSONFile("config.json");
	if (config != nullptr)
	{
		LoadConfig();
	}

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init();
		++item;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
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


	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %.2f Last sec frames: %i Vsync: %s ",
		avg_fps, last_frame_ms, prev_last_sec_frame_count, vsync ? "ON" : "OFF");
	window->SetTitle(title);

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

bool Application::LoadConfig()
{
	bool ret = true;
	std::list<Module*>::iterator item = list_modules.begin();
	while (item != list_modules.end())
	{
		(*item)->LoadConfig(config);
		++item;
	}
	return ret;
}

bool Application::SaveConfig()
{
	bool ret = true;
	std::list<Module*>::iterator item = list_modules.begin();
	while (item != list_modules.end())
	{
		(*item)->SaveConfig(config);
		++item;
	}
	return ret;
}



JSON_Object * Application::LoadJSONFile(const std::string & path)
{
	JSON_Value* value = json_parse_file(path.data());
	JSON_Object* object = json_value_get_object(value);

	if (value == nullptr || object == nullptr)
	{
		LOG("Error loading file %s", path);
	}

	return object;
}

void Application::Log(const char* fmt)
{
	ui->Log(fmt);
}

void Application::OpenWebsite(const std::string & link)
{
	ShellExecuteA(NULL, "open", link.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}