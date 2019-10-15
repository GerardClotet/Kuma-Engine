#ifndef _PANEL_INSPECTOR_H_
#define _PANEL_INSPECTOR_H_

#include <vector>

#include "Panel.h"
#include "Application.h"

#include "ImGui/imgui.h"
class GameObject;
class PanelInspector : public Panel
{
public:

	PanelInspector() {};
	PanelInspector(const char* name) : Panel(name) { App->saveLog("Inspector Panel Created"); };
	~PanelInspector() {};

	update_status Draw();


	void DisplayInspector();
	GameObject* RecursiveTreeNode(GameObject* parent);
};
#endif // !_PANEL_INSPECTOR_H_
