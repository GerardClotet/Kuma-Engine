#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);
	void GetWindowSize(uint &w, uint &h);
	void GetWindowsMinMax(uint& min_w, uint& min_h, uint& max_w, uint& max_h_);
	void SetWindowBrightness(float b);
	float GetWindowBrightness();
	int GetScreenWidth();
	int GetScreenHeight();
	void SetScreenWidth(uint);
	void SetScreenHeight(uint);
	uint GetRefreshRate();
	bool Get_if_FullScreen();
	bool Get_if_Resizable();
	bool Get_if_Borderless();
	bool Get_if_FullDesktop();
	void Set_FullScreen(bool s);
	void Set_Resizable(bool s);
	void Set_Borderless(bool s);
	void Set_FullDesktop(bool s);
	void LoadConfig(JSON_Object*& config);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	int screen_width;
	int screen_height;

	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool full_desktop = false;
public:
	/*int width;
	int height;*/
	float brightness;
	
	
};

#endif // __ModuleWindow_H__