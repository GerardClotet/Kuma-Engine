#ifndef PANELABOUT_H_
#define PANELABOUT_H_

#include "Panel.h"
#include "Application.h"
#include "Module.h"

class PanelAbout : public Panel
{
public:
	PanelAbout() {};
	PanelAbout(const char* name) :Panel(name) { App->saveLog("About Panel Created"); };
	~PanelAbout() {};

	update_status Draw();
	void DisplayAbout();

};




#endif // !PANELABOUT_H
