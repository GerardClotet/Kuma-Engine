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

	PanelHierarchy(const char* name) : Panel(name) { App->saveLog("Inspector Panel Created"); }
	~PanelHierarchy() {};

	update_status Draw();


	void DisplayHierarchy();
	void RecursiveTreeNode(const GameObject* parent);

	const GameObject* selected_game_obj;
};

#endif // !_PANEL_HIERARCHY_H__
