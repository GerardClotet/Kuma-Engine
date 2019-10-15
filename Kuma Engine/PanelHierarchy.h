#ifndef _PANEL_HIERARCHY_H__
#define _PANEL_HIERARCHY_H__


#include <vector>

#include "Panel.h"
#include "Application.h"
#include "ImGui/imgui.h"

class GameObject;
class PanelHierarchy : public Panel

{
public:
	PanelHierarchy() {};

	PanelHierarchy(const char* name) : Panel(name) { App->saveLog("Hierarchy Panel Created"); }
	~PanelHierarchy() {};

	update_status Draw();


	void DisplayHierarchy();
	void RecursiveTreeNode(GameObject* parent);

};

#endif // !_PANEL_HIERARCHY_H__
