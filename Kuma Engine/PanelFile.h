#ifndef PANELFILE_H
#define PANELFILE_H

#include "Panel.h"
#include <vector>
#include "Module.h"
#include "Application.h"
#include "ImGui/imgui.h"


class PanelFile : public Panel
{
public:
	PanelFile() {};
	PanelFile(const char* name) :Panel(name) {
		App->saveLog("File Panel Created");
	};
	~PanelFile();

	update_status Draw();
	void DisplayFiles();
	void DisplaySaveFiles();

	bool file_window = true;

};

#endif // !PANELFILE_H