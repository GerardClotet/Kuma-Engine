#include "PanelAbout.h"
#include "ModuleUI.h"

update_status PanelAbout::Draw()
{
	if (App->ui->about_window)
		DisplayAbout();
	return UPDATE_CONTINUE;
}

void PanelAbout::DisplayAbout()
{
	ImGui::Begin("About", &App->ui->about_window);

	ImGui::Separator();
	
	ImGui::Text("Kuma Engine %s", App->GetAppVersion());

	ImGui::End();
}
