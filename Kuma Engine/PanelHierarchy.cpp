#include "PanelHierarchy.h"
#include "ModuleUI.h"
#include "ImGui/imgui.h"

update_status PanelHierarchy::Draw()
{
	if (App->ui->hierarchy_window)
		DisplayHierarchy();

	return UPDATE_CONTINUE;
}

void PanelHierarchy::DisplayHierarchy()
{
	ImGui::Begin("Hierarchy", &App->ui->hierarchy_window);


	ImGui::End();
	
}
