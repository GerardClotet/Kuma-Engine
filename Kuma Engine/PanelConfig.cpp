#include "PanelConfig.h"
#include "ImGui/imgui.h"

#include "Application.h"

#include "ModuleUI.h"
#include "Module.h"

update_status PanelConfig::Draw()
{
	if (ImGui::Button(name))
	{
		
		configuration_window = true;
	}
	if (configuration_window)DisplayConfig();

	return UPDATE_CONTINUE;
}
void PanelConfig::DisplayConfig()
{
	ImGui::Begin("Configuration",&configuration_window);

	if (ImGui::BeginMenu("Options"))
	{
		if (ImGui::MenuItem("Set Defaults"))
		{
		}
		if (ImGui::MenuItem("Load"))
		{
		}
		if (ImGui::MenuItem("Save"))
		{
		}
		ImGui::EndMenu();
	}

	if (ImGui::CollapsingHeader("Application"))
	{
		static char name;
		if (ImGui::InputText("App Name", &name, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{

		}
		static char org_name;
		if (ImGui::InputText("Organization", &org_name, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			
			App->GetFramerateCap();
		}
		int max_fps = App->GetFramerateCap();
		if (ImGui::SliderInt("Max FPS", &max_fps, 0, 120))
		{
			App->vsync = true;
			App->SetFramerateCap(max_fps);
		}

		ImGui::Text("Limit Framerate:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", max_fps);
		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", App->ui->fps_log[App->ui->fps_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &App->ui->fps_log[0], App->ui->fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		ImGui::SameLine();
		App->ui->HelpMarker("framerato doesn't increase over 60,/n maybe cause the user screen refresh rate");
		sprintf_s(title, 25, "Milliseconds %0.1f", App->ui->ms_log[App->ui->ms_log.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &App->ui->ms_log[0], App->ui->ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	}
	if (ImGui::CollapsingHeader("Window"))
	{
		ImGui::Checkbox("Active", &activeWindow);
		if (!activeWindow) //needs no te fixed because we are not freeing memory
			exit(0);
		ImGui::Text("Icon:");
		ImGui::SameLine();

		if (ImGui::MenuItem("*default*"))
		{
			config_default = (config_default == false) ? true : false;


		}


	}
	if (ImGui::CollapsingHeader("File System"))
	{

	}
	if (ImGui::CollapsingHeader("Input"))
	{

	}
	if (ImGui::CollapsingHeader("Hardware"))
	{
		ImGui::Text("SDL Version: %s", App->hardware->sdl_version);
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "CPUs:");
		ImGui::SameLine();
		ImGui::Text("  %i", App->hardware->cpu_cores);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Cache: ");
		ImGui::SameLine();
		ImGui::Text(" %ukb", App->hardware->l1_cpu_cache_bytes);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "System RAM:");
		ImGui::SameLine();
		ImGui::Text("%.1fGb", App->hardware->system_RAM_gb);

		if (App->hardware->rdtsc)  ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "RDTSC");
		ImGui::SameLine();
		if (App->hardware->altivec) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "AltiVec");
		ImGui::SameLine();
		if (App->hardware->mmx) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "MMX");
		ImGui::SameLine();
		if (App->hardware->has3D_now) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "3DNow");
		ImGui::SameLine();
		if (App->hardware->sse)ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE");
		ImGui::SameLine();
		if (App->hardware->sse2) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE2");
		/*ImGui::SameLine();*/
		if (App->hardware->sse3) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE3");
		ImGui::SameLine();
		if (App->hardware->sse41) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE41");
		ImGui::SameLine();
		if (App->hardware->sse42)ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE42");
		ImGui::SameLine();
		if (App->hardware->avx)ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "AVX");
		ImGui::SameLine();
		if (App->hardware->avx2) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "AVX2");


	}

	ImGui::End();
}
