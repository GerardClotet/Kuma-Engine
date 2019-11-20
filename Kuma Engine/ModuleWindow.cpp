#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1

		/*SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);*/

		if(WIN_FULLSCREEN == true ||fullscreen)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true || resizable)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true || borderless)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true || full_desktop)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, flags);
		SDL_SetWindowBrightness(window, brightness);
		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	//Icon
	SDL_Surface* icon = SDL_LoadBMP("Assets/icons/Icon.bmp");
	SDL_SetWindowIcon(window, icon);

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	if (title != nullptr)
	{
		SDL_SetWindowTitle(window, title);
	}
}

void ModuleWindow::GetWindowSize(uint &w, uint &h)
{
	 w = screen_width * SCREEN_SIZE;
	 h = screen_height * SCREEN_SIZE;
}

void ModuleWindow::GetWindowsMinMax(uint& min_w, uint& min_h, uint& max_w, uint& max_h)
{
	min_w = 600;
	min_h = 480;
	max_w= 3000;
	max_h = 2000;
}

void ModuleWindow::SetWindowBrightness(float b)
{
	SDL_SetWindowBrightness(window, b);
}

float ModuleWindow::GetWindowBrightness()
{
	return SDL_GetWindowBrightness(window);
}


int ModuleWindow::GetScreenWidth()
{
	return screen_width;
}

int ModuleWindow::GetScreenHeight()
{
	return screen_height;
}

void ModuleWindow::SetScreenWidth(uint w)
{
	SDL_SetWindowSize(window, w, GetScreenHeight());
	screen_width = w;
}

void ModuleWindow::SetScreenHeight(uint h)
{
	SDL_SetWindowSize(window, GetScreenWidth(), h);
	screen_height = h;

}

uint ModuleWindow::GetRefreshRate()
{
	uint ret = 0;

	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		LOG("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
	}
	else ret = dm.refresh_rate;

	return ret;
}

bool ModuleWindow::Get_if_FullScreen()
{
	return fullscreen;
}

bool ModuleWindow::Get_if_Resizable()
{
	return resizable;
}

bool ModuleWindow::Get_if_Borderless()
{
	return borderless;
}

bool ModuleWindow::Get_if_FullDesktop()
{
	return full_desktop;
}

void ModuleWindow::Set_FullScreen(bool set)
{
	if (set)
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		fullscreen = set;
		full_desktop = false;
	}
	else
	{
		SDL_SetWindowFullscreen(window, 0);
		fullscreen = set;
	}
}

void ModuleWindow::Set_Resizable(bool set)
{
	resizable = set;
	SDL_SetWindowResizable(window, (SDL_bool)set);
}

void ModuleWindow::Set_Borderless(bool set)
{
	if (set != borderless && fullscreen == false && full_desktop == false)
	{
		borderless = set;
		SDL_SetWindowBordered(window, (SDL_bool)!borderless);
	}
}

void ModuleWindow::Set_FullDesktop(bool set)
{

	if (set)
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		full_desktop = set;
		fullscreen = false;
	}
	else
	{
		SDL_SetWindowFullscreen(window, 0);
		full_desktop = set;
	}
}
void ModuleWindow::LoadConfig(JSON_Object *& config)
{
	screen_width = json_object_dotget_number(config, "Configuration.Window.Width");
	screen_height = json_object_dotget_number(config, "Configuration.Window.Height");
	brightness = json_object_dotget_number(config, "Configuration.Window.Brightness");
	fullscreen = json_object_dotget_boolean(config, "Configuration.Window.Fullscreen");
	borderless = json_object_dotget_boolean(config, "Configuration.Window.Borderless");
	resizable = json_object_dotget_boolean(config, "Configuration.Window.Resizable");
	full_desktop = json_object_dotget_boolean(config, "Configuration.Window.FullDesktop");

	if (!firstLoadConfig)
	{
		SetScreenWidth(screen_width);
		SetScreenHeight(screen_height);
		SetWindowBrightness(brightness);
		Set_FullScreen(fullscreen);
		Set_Borderless(borderless);
		Set_Resizable(resizable);
		Set_FullDesktop(full_desktop);
		
	}

	if (firstLoadConfig)
		firstLoadConfig = false;
}

void ModuleWindow::SaveConfig(JSON_Object*& config, std::string path)
{
	json_object_dotset_number(config, "Configuration.Window.Width", screen_width);
	json_object_dotset_number(config, "Configuration.Window.Height", screen_height);
	json_object_dotset_boolean(config, "Configuration.Window.Borderless", borderless);
	json_object_dotset_number(config, "Configuration.Window.Brightness", brightness);
	json_object_dotset_boolean(config, "Configuration.Window.Fullscreen", fullscreen);
	json_object_dotset_boolean(config, "Configuration.Window.FullDesktop", full_desktop);
	json_object_dotset_boolean(config, "Configuration.Window.Resizable", resizable);


}
