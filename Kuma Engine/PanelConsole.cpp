#include "PanelConsole.h"
#include "ImGui/imgui.h"
#include "Panel.h"
#include "Application.h"
update_status PanelConsole::Draw()
{



	if (ImGui::Button(name))
	{

		console_window = true;
	}
	if (console_window)DisplayConsole();
	return UPDATE_CONTINUE;
}


void PanelConsole::DisplayConsole()
{
	ImGui::Begin("Console", &console_window);


	ImGui::End();
}