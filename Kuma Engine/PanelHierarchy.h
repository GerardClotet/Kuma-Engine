#ifndef PANEL_HIERARCHY_H
#define PANEL_HIERARCHY_H

#include "Panel.h"
#include "Application.h"

class PanelHierarchy : public Panel
{
public:
	PanelHierarchy() {};
	PanelHierarchy(const char* name) : Panel(name) { App->saveLog("Hierarchy Panel Created"); };

	update_status Draw();

	void DisplayHierarchy();
};

#endif // !PANEL_HIERARCHY_H
