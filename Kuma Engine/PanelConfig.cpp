#include "PanelConfig.h"
#include "Globals.h"
#include "ModuleRenderer3D.h"
#include "ModuleUI.h"
#include "ModuleInput.h"
#include "ModuleHardware.h"
#include "ModuleImporter.h"
#include "ModuleCamera3D.h"
#include "ModuleSceneIntro.h"
#include "Component_Camera.h"
#include "GameObject.h"
#include "Component_Material.h"

#include "mmgr/mmgr.h"

using namespace std;

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
			JSON_Object* config_default = App->LoadJSONFile("config.json");
			if (config_default == nullptr)
			{
				config_default = App->LoadJSONFile("Configuration/config.json");
			}
			std::list<Module*>::iterator item = App->list_modules.begin();
			while (item != App->list_modules.end())
			{
				(*item)->LoadConfig(config_default);
				++item;
			}
		}
		if (ImGui::MenuItem("Load"))
		{
			std::string file = SelectFile();
			if (file.length()!=0)
			{
				JSON_Object* config = App->LoadJSONFile(file);

				std::list<Module*>::iterator item = App->list_modules.begin();
				while (item != App->list_modules.end())
				{
					(*item)->LoadConfig(config);
					++item;
				}
				App->LoadConfig(config);
			}
			
		}
		if (ImGui::MenuItem("Save"))
		{
			std::string file = SelectFile();

			if (file.length() != 0)
			{
				App->save_value = json_parse_file(file.data());
				App->save_object = json_object(App->save_value);
				std::list<Module*>::iterator item = App->list_modules.begin();
				while (item != App->list_modules.end())
				{
					(*item)->SaveConfig(App->save_object, file);
					++item;
				}
				App->SaveConfig(App->save_object, file);
				App->SaveConfigFinish(file);
			}
		}
		ImGui::EndMenu();
	}

	if (ImGui::CollapsingHeader("Application"))
	{
		static char app_name[150];
		strcpy_s(app_name, 150, App->GetAppName());
		if (ImGui::InputText("App Name", app_name, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			App->SetAppName(app_name);
			
		
		static char org_name[150];
		strcpy_s(org_name, 150, App->GetOrganizationName());
		if (ImGui::InputText("Organization", org_name, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			App->SetOrgName(org_name);
			
		
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

		App->memory.push_back(m_getMemoryStatistics().totalActualMemory);

		if (App->memory.size() > 100)
		{
			App->memory.erase(App->memory.begin());
		}

		static int speed = 0;


		std::vector<float> mem =  App->memory;
		
		
		if (++speed > 20)
		{
			speed = 0; 
			
			if (mem.size() == 100)
			{
				for (uint i = 0; i < 100 - 1; ++i)
				{
					mem[i] = mem[i + 1];
				}

				mem[100 - 1] = stats.totalReportedMemory;

			}
			else
				mem.push_back((float)stats.totalReportedMemory);
		}


		sprintf_s(title, 25, "mem %0.1f", mem[mem.size() - 1]);




		ImGui::PlotHistogram("##mem", &(mem[0]), mem.size(), 0, title, 0.0f, stats.peakActualMemory, ImVec2(310, 100));

		

		ImGui::Text("Total Reported Mem: %d", m_getMemoryStatistics().totalReportedMemory);
		ImGui::Text("Peak Reported Mem: %d", m_getMemoryStatistics().peakReportedMemory);
		ImGui::Text("Total Actual Mem: %d", m_getMemoryStatistics().totalActualMemory);
		ImGui::Text("Peak Actual Mem: %d", m_getMemoryStatistics().peakActualMemory);
		ImGui::Text("Accumulated Reported Mem: %d", m_getMemoryStatistics().accumulatedReportedMemory);
		ImGui::Text("Accumulated Actual Mem: %d", m_getMemoryStatistics().accumulatedActualMemory);
		ImGui::Text("Accumulated Alloc Unit Count: %d", m_getMemoryStatistics().accumulatedAllocUnitCount);
		ImGui::Text("Total Alloc Unit Count: %d", m_getMemoryStatistics().totalAllocUnitCount);
		ImGui::Text("Peak Alloc Unit Count: %d", m_getMemoryStatistics().peakAllocUnitCount);

	}
	if (ImGui::CollapsingHeader("Window"))
	{
		//ImGui::Checkbox("Active", &activeWindow);
		//if (!activeWindow) //needs no te fixed because we are not freeing memory
		//	exit(0);
		ImGui::Text("Icon:");
		ImGui::SameLine();

		if (ImGui::MenuItem("*default*"))
		{
			config_default = (config_default == false) ? true : false;
		}
		float brightness = App->window->GetWindowBrightness();
		if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 2.0f))
			App->window->SetWindowBrightness(brightness);

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
		ImGui::Checkbox("Active", &activeFileSystem);
		ImGui::Text("Base Path:");
		basePath = SDL_GetBasePath();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), basePath);
		ImGui::Text("Read Paths:");
		//TODO:/ WE NEED TO PUT HERE SOMETHING THAT I DON'T KNOW NOW WHAT'S IT
		ImGui::Text("Write Path:");
		//TODO:/ WE NEED TO PUT HERE SOMETHING THAT I DON'T KNOW NOW WHAT'S IT
	}
	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Checkbox("Active", &activeInput);

		ImGui::Text("Mouse Position: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i , %i", App->input->GetMouseX(), App->input->GetMouseY());


		ImGui::Text("Mouse Motion: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i , %i", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());

		ImGui::Text("Mouse Wheel: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", App->input->GetMouseWheel());

		ImGui::Separator();
		ImGui::BeginChild("scrolling");

		ImGui::TextUnformatted(input_buf.begin());

		if (scrollToBottom)
			ImGui::SetScrollHere(1.0f);
		scrollToBottom = false;

		ImGui::EndChild();

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

		ImGui::Text("Caps: ");
		ImGui::SameLine();
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

		ImGui::Separator();
		ImGui::Text("GPU:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "vendor %u device %u",App->hardware->gpu_vendor,App->hardware->gpu_device);
		ImGui::Text("Brand:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", App->hardware->gpu_brand);


		ImGui::Text("VRAM Budget:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.2f MB", App->hardware->vram_mb_budget);

		ImGui::Text("VRAM Usage:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.2f MB", App->hardware->vram_mb_usage);


		ImGui::Text("VRAM Available:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.2f MB", App->hardware->vram_mb_available);

		ImGui::Text("VRAM Reserved:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.2f MB", App->hardware->vram_mb_reserved);
	}

	if (ImGui::CollapsingHeader("Renderer"))
	{

		if (ImGui::Checkbox("GL_DEPTH_TEST", &GL_Depth_test))
		{
			if (GL_Depth_test)
				glEnable(GL_DEPTH_TEST), LOG("Enabled ColorMaterial");

			else if (!GL_Depth_test)
				glDisable(GL_DEPTH_TEST), LOG("Disabled ColorMaterial");
		}

		ImGui::Separator();

		if (ImGui::Checkbox("GL_CULL_FACE", &GL_Cull_face))
		{
			if (GL_Cull_face)
				glEnable(GL_CULL_FACE), LOG("Enabled Cullface");

			else if (!GL_Cull_face)
				glDisable(GL_CULL_FACE), LOG("Disabled Cullface");
		}

		ImGui::Separator();



		if (ImGui::Checkbox("GL_LIGHTING", &GL_Lightning))
		{
			if (GL_Lightning)
				glEnable(GL_LIGHTING), LOG("Enabled Lightning");

			else if (!GL_Lightning)
				glDisable(GL_LIGHTING), LOG("Disabled Lightning");

		}

		ImGui::Separator();

		if (ImGui::Checkbox("GL_COLOR_MATERIAL", &GL_Color_material))
		{
			if (GL_Color_material)
				glEnable(GL_COLOR_MATERIAL), LOG("Enabled ColorMaterial");

			else if (!GL_Color_material)
				glDisable(GL_COLOR_MATERIAL), LOG("Disabled ColorMaterial");
		}


		ImGui::Separator();
		if (ImGui::Checkbox("GL_POLYGON_MODE", &GL_Wireframe))
		{
			/*if (GL_Wireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			else if (!GL_Wireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

			if (!GL_Fill && !GL_Wireframe)
				GL_Fill = true;
		}
		ImGui::SameLine();
		App->ui->HelpMarker("Polygon mode refers to wireframe mode");
		if (ImGui::Checkbox("GL_FILL", &GL_Fill))
		{
			if (!GL_Wireframe) GL_Wireframe = true;
			LOG("GL wireframe %s", GL_Fill ? "ON" : "OFF");
		}

//		ImGui::ColorEdit4()
		ImGui::Separator();
		
		ImGui::ColorEdit4("Wire Color", (float*)& App->importer->wire_color);
		ImGui::ColorEdit4("Fill Color", (float*)& App->importer->fill_color);
		ImGui::ColorEdit4("Vertex Color", (float*)& App->importer->vertex_color);
		ImGui::ColorEdit4("Face normal Color", (float*)& App->importer->face_normal_color);
		ImGui::ColorEdit4("Vertex normal Color", (float*)& App->importer->vertex_normal_color);

		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Checkbox("Debug Mouse Picking", &App->camera->pickedRayCast);
		ImGui::Spacing();
		ImGui::ColorEdit4("Raycast Color", (float*)& App->camera->ray_color);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Checkbox("Quadtree", &App->scene_intro->printQuadtree);
		ImGui::Spacing();
		ImGui::Spacing();
	}

	if (ImGui::CollapsingHeader("Scene Camera"))
	{
		if (ImGui::DragFloat("Near Plane", &near_plane, 1, 0.1f, far_plane - 0.1f, "%.1f"))
		{
			App->camera->camera_fake->frustum.nearPlaneDistance = near_plane;
		}

		ImGui::Spacing();


		if (ImGui::DragFloat("Far Plane", &far_plane, 1, near_plane + 0.1f, 500, "%.1f"))
		{
			App->camera->camera_fake->frustum.farPlaneDistance = far_plane;
		}

		ImGui::Spacing();
		ImGui::Spacing();



		if (ImGui::DragFloat("FOV Horizontal", &horizontal_fov, 1, 1, 163, "%.1f"))
		{
			App->camera->camera_fake->frustum.horizontalFov = horizontal_fov * DEGTORAD;
			App->camera->camera_fake->SetAspectRatio(16, 9, true);
			vertical_fov = App->camera->camera_fake->frustum.verticalFov * RADTODEG;
		}



		if (ImGui::DragFloat("FOV Vertical", &vertical_fov, 1, 1, 150, "%.1f"))
		{
			App->camera->camera_fake->frustum.verticalFov = vertical_fov * DEGTORAD;
			App->camera->camera_fake->SetAspectRatio(16, 9);
			horizontal_fov = App->camera->camera_fake->frustum.horizontalFov * RADTODEG;
		}
	}


	ImGui::End();
}

void PanelConfig::AddInputToBuffer(const char * input)
{
	input_buf.append(input);
	scrollToBottom = true;
}

bool PanelConfig::Getwireframe()
{
	return GL_Wireframe;
}

bool PanelConfig::GetFill()
{
	return GL_Fill;
}

std::string PanelConfig::SelectFile()
{
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "JSON Files\0*.json";  //Add a filter, so you can limite the files you want. In this case you can only selet .json files
	ofn.lpstrFile = filename; //This will recieve the name
	ofn.nMaxFile = MAX_PATH; //max size in characters of the path
	ofn.lpstrTitle = "Select a .json file to load"; //the title of the dialogue box
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST; //Flags that do flag things :)

	if (GetOpenFileNameA(&ofn)) //the function that opens the file folder
	{
		LOG("%s is the file you have selected", filename);
	}
	else
	{
		//If you messed up things above, the exact error will be show in the output console
		switch (CommDlgExtendedError())
		{
		case CDERR_DIALOGFAILURE: LOG("CDERR_DIALOGFAILURE\n");   break;
		case CDERR_FINDRESFAILURE: LOG("CDERR_FINDRESFAILURE\n");  break;
		case CDERR_INITIALIZATION: LOG("CDERR_INITIALIZATION\n");  break;
		case CDERR_LOADRESFAILURE: LOG("CDERR_LOADRESFAILURE\n");  break;
		case CDERR_LOADSTRFAILURE: LOG("CDERR_LOADSTRFAILURE\n");  break;
		case CDERR_LOCKRESFAILURE: LOG("CDERR_LOCKRESFAILURE\n");  break;
		case CDERR_MEMALLOCFAILURE: LOG("CDERR_MEMALLOCFAILURE\n"); break;
		case CDERR_MEMLOCKFAILURE: LOG("CDERR_MEMLOCKFAILURE\n");  break;
		case CDERR_NOHINSTANCE: LOG("CDERR_NOHINSTANCE\n");     break;
		case CDERR_NOHOOK: LOG("CDERR_NOHOOK\n");          break;
		case CDERR_NOTEMPLATE: LOG("CDERR_NOTEMPLATE\n");      break;
		case CDERR_STRUCTSIZE: LOG("CDERR_STRUCTSIZE\n");      break;
		case FNERR_BUFFERTOOSMALL: LOG("FNERR_BUFFERTOOSMALL\n");  break;
		case FNERR_INVALIDFILENAME: LOG("FNERR_INVALIDFILENAME\n"); break;
		case FNERR_SUBCLASSFAILURE: LOG("FNERR_SUBCLASSFAILURE\n"); break;
		default: LOG("You cancelled.\n");
		}
		
	}

	return filename;
}
