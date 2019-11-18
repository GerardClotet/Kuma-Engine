#ifndef PANELSCENE_H
#define PANELSCENE_H

#include "Panel.h"
#include "ImGui/imgui.h"
#include <string>

class ImVec2;

class PanelScene : public Panel {
public:
	PanelScene() {};
	PanelScene(const char* name);
	~PanelScene() {};

	update_status Draw();

public:
	std::string name;
	ImVec2 size;
};
#endif // !PANELSCENE_H
