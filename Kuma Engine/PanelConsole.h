#ifndef PANEL_CONSOLE_H_
#define PANEL_CONSOE_H_

#include "Panel.h"
#include <vector>
#include "Module.h"
#include "ImGui/imgui.h"

class PanelConsole : public Panel
{
public:
	PanelConsole() {};

	PanelConsole(const char* name) :Panel(name) {};
	~PanelConsole() {};

	update_status Draw();
	void DisplayConsole();

	void    Clear();
	void AddLog(const char* fmt,...);
	
	bool console_window = true;

public:
	ImGuiTextBuffer		Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets;        // Index to lines offset
	bool                ScrollToBottom;

};
#endif