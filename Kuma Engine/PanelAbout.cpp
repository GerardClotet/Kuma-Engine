#include "PanelAbout.h"
#include "ModuleUI.h"
#include "ModuleHardware.h"
#include "ImGui/imgui.h"

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
	ImGui::Text("The new oriental engine");
	if (ImGui::Button("By Pol Recasens & Gerard Clotet"))
		App->OpenWebsite("https://github.com/GerardClotet/Kuma-Engine");

	ImGui::NewLine();

	ImGui::Text("3rd Party Libraries used");
	ImGui::BulletText("SDL %s", App->hardware->sdl_version);
	ImGui::BulletText("Glew 2.1.0");
	ImGui::BulletText("PCG Random Number Generation 0.98");
	ImGui::BulletText("MathGeoLib");
	ImGui::BulletText("ImGui 1.73");
	ImGui::BulletText("mmgr");
	ImGui::BulletText("gpudetect");
	ImGui::BulletText("Assimp");
	ImGui::BulletText("Bass");
	ImGui::BulletText("DevIL");
	ImGui::BulletText("imGuizmo");
	ImGui::BulletText("Parson");
	ImGui::BulletText("Rapid json");
	ImGui::NewLine();

	ImGui::Text("%s", App->GetLicense());
	

	ImGui::EndChild();
	ImGui::End();
}
