#ifndef PANEL_CONSOLE_H_
#define PANEL_CONSOE_H_

#include "Panel.h"
#include <vector>
#include "Module.h"
#include "Application.h"
#include "ImGui/imgui.h"

class PanelConsole : public Panel
{
public:
	PanelConsole() {};

	PanelConsole(const char* name) :Panel(name) { App->saveLog("Console Panel Created"); };
	~PanelConsole() {};

	update_status Draw();
	void DisplayConsole();

	void    Clear();
	void AddLog(const char* fmt,...);
	
	bool console_window = true;

public:
	ImGuiTextBuffer		Buf;
	ImGuiTextFilter     Filter;
	bool                ScrollToBottom;

};
#endif