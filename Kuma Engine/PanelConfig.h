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

class PanelConfig : public Panel
{
public:
	PanelConfig() {};
	PanelConfig(const char* name) :Panel(name) {
	
	};
	~PanelConfig() {};

	update_status Draw();
	void DisplayConfig();


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

	uint w = App->window->GetScreenWidth();
	uint h = App->window->GetScreenHeight();

private:
	std::string SelectFile(); //This allows you to select a .json file
	char filename[MAX_PATH]; //This is the variable that will recieve the file path
	OPENFILENAME ofn; //struct with all the info needed to create the dialogue to select the file
};



#endif // !PANEL_CONFIG_H_
