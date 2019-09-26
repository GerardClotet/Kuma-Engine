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

	screen_width = SCREEN_WIDTH * SCREEN_SIZE;
	screen_height = SCREEN_HEIGHT * SCREEN_SIZE;
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true ||fullscreen)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true||resizable)
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

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

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
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::GetWindowSize(uint w, uint h)
{
	 w = SCREEN_WIDTH * SCREEN_SIZE;
	 h = SCREEN_HEIGHT * SCREEN_SIZE;
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
}

void ModuleWindow::SetScreenHeight(uint h)
{
	SDL_SetWindowSize(window, GetScreenWidth(), h);

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
	if (fullscreen != set)
	{
		if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0)
			LOG("Could not switch to fullscreen: %s\n", SDL_GetError());

		full_desktop = false;
	}
	else
	{
		if (SDL_SetWindowFullscreen(window, 0) != 0)
			LOG("Could not switch to windowed: %s\n", SDL_GetError());
	}
}

void ModuleWindow::Set_Resizable(bool set)
{
	resizable = set;
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
	if (set != full_desktop)
	{
		full_desktop = set;
		if (full_desktop == true)
		{
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
				LOG("Could not switch to fullscreen desktop: %s\n", SDL_GetError());
			fullscreen = false;
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) != 0)
				LOG("Could not switch to windowed: %s\n", SDL_GetError());
		}
	}
}
