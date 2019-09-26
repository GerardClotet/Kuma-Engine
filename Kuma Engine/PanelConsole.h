#ifndef PANEL_CONSOLE_H_
#define PANEL_CONSOE_H_

#include "Panel.h"
#include <vector>
#include "Module.h"


class PanelConsole : public Panel
{
public:
	PanelConsole() {};

	PanelConsole(const char* name) :Panel(name) {};
	~PanelConsole() {};

	update_status Draw();
	void DisplayConsole();
	bool console_window = false;
};
#endif