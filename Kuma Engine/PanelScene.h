#ifndef PANELSCENE_H
#define PANELSCENE_H

#include "Panel.h"
#include "ImGui/imgui.h"

class ImVec2;

class PanelScene : public Panel {
public:
	PanelScene() {};
	PanelScene(const char* name) : Panel(name) {};
	~PanelScene() {};

	update_status Draw();

public:
	ImVec2 size;
};
#endif // !PANELSCENE_H
