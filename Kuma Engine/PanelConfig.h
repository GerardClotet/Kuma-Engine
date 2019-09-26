#ifndef PANEL_CONFIG_H_
#define PANEL_CONFIG_H_

#include "Panel.h"
#include <vector>
#include "Module.h"
class PanelConfig : public Panel
{
public:
	PanelConfig() {};
	PanelConfig(const char* name) :Panel(name) {};
	~PanelConfig() {};

	update_status Draw();
	void DisplayConfig();


private:
	bool configuration_window = false;
	bool open = false;
	//std::vector<float> fps_log;
	//std::vector<float> ms_log;
	bool activeWindow = true;
	bool config_default = false;

};



#endif // !PANEL_CONFIG_H_
