#include "PanelConfig.h"
#include "ImGui/imgui.h"

#include "Application.h"

#include "mmgr///mmgr.h"
#include "ModuleWindow.h"
update_status PanelConfig::Draw()
{
	
	if (App->ui->configuration_window)DisplayConfig();

	return UPDATE_CONTINUE;
}
void PanelConfig::DisplayConfig()
{
	ImGui::Begin("Configuration",&App->ui->configuration_window);

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

		sMStats stats = m_getMemoryStatistics();
		static int speed = 0;
		static std::vector<float> memory(100);
		if (++speed > 20)
		{
			speed = 0;
			if (memory.size() == 100)
			{
				for (uint i = 0; i < 100 - 1; ++i)
				{
					memory[i] = memory[i + 1];
				}

				memory[100 - 1] = stats.totalReportedMemory;

			}
			else
				memory.push_back((float)stats.totalReportedMemory);
		}
		ImGui::PlotHistogram("##memory", &memory[0], memory.size(), 0, "Mmemory Consumption,", 0.0f, (float)stats.peakReportedMemory, ImVec2(310, 100));

		ImGui::Text("Total Reported Memory: %u", stats.totalReportedMemory);

		ImGui::Text("Total Actual Mem: %u", stats.totalActualMemory);
		ImGui::Text("Peak Reported Mem: %u", stats.peakReportedMemory);
		ImGui::Text("Peak Actual Mem: %u", stats.peakActualMemory);
		ImGui::Text("Accumulated Reported Mem: %u", stats.accumulatedReportedMemory);
		ImGui::Text("Accumulated Actual Mem: %u", stats.accumulatedActualMemory);
		ImGui::Text("Accumulated Alloc Unit Count: %u", stats.accumulatedAllocUnitCount);
		ImGui::Text("Total Alloc Unit Count: %u", stats.totalAllocUnitCount);
		ImGui::Text("Peak Alloc Unit Count: %u", stats.peakAllocUnitCount);

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
		float brightness = App->window->GetWindowBrightness();
		if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f))
			App->window->SetWindowBrightness(brightness);

		uint w=0;
		uint h=0;
		uint min_w=0, min_h=0, max_w=0, max_h=0;
		App->window->GetWindowSize(w, h);
		App->window->GetWindowsMinMax(min_w, min_h, max_w, max_h);
		if(ImGui::SliderInt("Widht", (int*)&w, min_w, max_w));
			App->window->SetScreenWidth(w);
		

		if (ImGui::SliderInt("Height", (int*)& h, min_h, max_h))
			App->window->SetScreenHeight(h);

		ImGui::Text("Refresh Rate:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%u", App->window->GetRefreshRate());
		
		bool fullscreen = App->window->Get_if_FullScreen();
		bool resizable = App->window->Get_if_Resizable();
		bool borderless = App->window->Get_if_Borderless();
		bool full_desktop = App->window->Get_if_FullDesktop();

		if (ImGui::Checkbox("Fullscreen", &fullscreen))
			App->window->Set_FullScreen(fullscreen);

		ImGui::SameLine();
		if (ImGui::Checkbox("Resizable", &resizable))
			App->window->Set_Resizable(resizable);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Restart to apply");

		if (ImGui::Checkbox("Borderless", &borderless))
			App->window->Set_Borderless(borderless);

		ImGui::SameLine();
		if (ImGui::Checkbox("Full Desktop", &full_desktop))
			App->window->Set_FullDesktop(full_desktop);
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
