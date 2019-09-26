#include "PanelConsole.h"
#include "ImGui/imgui.h"
#include "Panel.h"
#include "Application.h"
update_status PanelConsole::Draw()
{




	if (App->ui->console_window)DisplayConsole();
	return UPDATE_CONTINUE;
}


void PanelConsole::DisplayConsole()
{
	ImGui::Begin("Console", &App->ui->console_window);


	ImGui::End();
}