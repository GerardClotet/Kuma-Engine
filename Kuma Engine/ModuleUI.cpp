#include "Globals.h"
#include "ModuleUI.h"
#include "Application.h"
#include "ModuleHardware.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	fps_log.resize(100);
	ms_log.resize(100);
}

ModuleUI::~ModuleUI()
{}


bool ModuleUI::Start()
{
	
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	  // Setup Dear ImGui style
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

	return true;

}

bool ModuleUI::CleanUp()
{
	LOG("Unloading UI module");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}

update_status ModuleUI::Update(float dt)
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();



	if (ImGui::BeginMainMenuBar())
	{
		//Begin Menu
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit"))
			{
				return UPDATE_STOP;
			}
			FileScreen();

			ImGui::EndMenu();
		}


		if (ImGui::BeginMenu("View"))
		{
			ViewScreen();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			HelpScreen();

			ImGui::EndMenu();
		}


		ImGui::EndMainMenuBar();
	}


	//call demo function
	if (demoWindow)
		ImGui::ShowDemoWindow();

	//call console function
	if (console_window)
		DisplayConsole();
		
	//call config window function
	if (configuration_window)
		DisplayConfig();

	if (config_default)
		DisplayConfigDefault();
		

	if (show_obj_edit_window)
	{
		ObjectEditor();
	}

	ImGui::Render();

	
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return UPDATE_CONTINUE;
}



void ModuleUI::HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void ModuleUI::ObjectEditor()
{
	ImGui::Begin("Object Editor",&show_obj_edit_window);

	if (ImGui::CollapsingHeader("Create"))

	{
		if (ImGui::TreeNode("Create"))
		{

			
			static float vec4f[4] = { 0, 0, 0,0 };
			 
			ImGui::Text("Radius");
			ImGui::InputFloat4("X , Y , Z , radius",vec4f );
	

		if(	ImGui::Button("Create Sphere"))
			{
			Sphere es({ vec4f[0],vec4f[1],vec4f[2] }, vec4f[3]);
			spherelist.push_back(es);

			}
			ImGui::TreePop();
		}
	}
	

	ImGui::End();

}

void ModuleUI::AddFPS(float fps, float ms)
{
	static int count=0;
	if (count = 100)
	{
		for (int i = 0; i < 100-1; ++i)
		{
			fps_log[i] = fps_log[i + 1];
			ms_log[i] = ms_log[i + 1];
		}
	}

	else ++count;

	fps_log[count - 1] = fps;
	ms_log[count - 1] = ms;

}

void ModuleUI::DisplayConsole()
{
	ImGui::Begin("Console", &console_window);


	ImGui::End();

}


void ModuleUI::DisplayConfig()
{
	ImGui::Begin("Configuration", &configuration_window);

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

		}
		int max_fps = App->GetFramerateCap();
		if (ImGui::SliderInt("Max FPS", &max_fps, 0, 120))
		{
			App->vsync = true;
			App->SetFramerateCap(max_fps);
		}
		
		ImGui::Text("Limit Framerate:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i",max_fps);
		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		ImGui::SameLine();
		HelpMarker("framerato doesn't increase over 60,/n maybe cause the user screen refresh rate");
		sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
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
		ImGui::Text("  %i" , App->hardware->cpu_cores);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Cache: ");
		ImGui::SameLine();
		ImGui::Text(" %ukb", App->hardware->l1_cpu_cache_bytes);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "System RAM:"); 
		ImGui::SameLine(); 
		ImGui::Text("%.1fGb", App->hardware->system_RAM_gb);

		if(App->hardware->rdtsc)  ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "RDTSC");
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


void ModuleUI::HelpScreen()
{
	if (ImGui::MenuItem("Gui Demo"))
	{
		demoWindow = (demoWindow == false) ? true : false;
	}

	if (ImGui::MenuItem("Documentation"))
	{

	}

	if (ImGui::MenuItem("Download latest"))
	{

	}

	if (ImGui::MenuItem("Report a bug"))
	{

	}
	if (ImGui::MenuItem("About"))
	{

	}
}

void ModuleUI::ViewScreen()
{
	if (ImGui::MenuItem("Console"))
	{
		console_window = (console_window == false) ? true : false;

	}
	ImGui::SameLine();
	ImGui::TextDisabled("1");


	if (ImGui::MenuItem("Configuration"))
	{
		configuration_window = (configuration_window == false) ? true : false;
		//this is the configuration
	}
	ImGui::SameLine();
	ImGui::TextDisabled("4");
}

void ModuleUI::FileScreen()
{
	//TODO:/ WE NEED TO PUT HERE THE DIFFERENT OPTIONS OF THE MENU, AS OPEN, SAVE, QUIT...
	ImGui::SameLine();
	ImGui::TextDisabled("ESC");

}

void ModuleUI::DisplayConfigDefault()
{
	ImGui::Begin("Load File", &config_default);

	ImGui::End();
}

