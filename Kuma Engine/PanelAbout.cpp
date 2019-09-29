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
	ImGui::BeginChild("scrolling");

	ImGui::Separator();
	
	ImGui::Text("Kuma Engine %s", App->GetAppVersion());
	ImGui::Text("On line description must be filled here!!!");
	ImGui::Text("By Pol Recasens & Gerard Clotet(need to put github repository)");

	ImGui::NewLine();

	ImGui::Text("3rd Party Libraries used");
	ImGui::BulletText("SDL %s", App->hardware->sdl_version);
	ImGui::BulletText("Glew 2.1.0");
	ImGui::BulletText("PCG Random Number Generation 0.98");
	ImGui::BulletText("MathGeoLib");
	ImGui::BulletText("ImGui 1.73");

	ImGui::NewLine();

	ImGui::Text("%s", App->GetLicense());
	

	ImGui::EndChild();
	ImGui::End();
}
