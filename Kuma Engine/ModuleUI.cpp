#include "Globals.h"
#include "ModuleUI.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleHardware.h"
#include "PanelConfig.h"
#include "PanelConsole.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	fps_log.resize(100);
	ms_log.resize(100);
	
}

ModuleEditor::~ModuleEditor()
{}


bool ModuleEditor::Start()
{

	
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	  // Setup Dear ImGui style
	ImGui::StyleColorsEdited();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();
	panel_list.push_back(config_p =new PanelConfig("Configuration"));
	panel_list.push_back(console_p =new PanelConsole("Console"));
	console_window = true;
	return true;

}

bool ModuleEditor::CleanUp()
{
	LOG("Unloading UI module");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}

update_status ModuleEditor::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;


	//Change style color with hotkey
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		changeColor = (changeColor == false) ? true : false;
		if (changeColor)
		ImGui::StyleColorsDark();
		else
			ImGui::StyleColorsEdited();
	}

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	for (std::list<Panel*>::iterator item = panel_list.begin(); item != panel_list.end(); ++item)
	{
		ret = (*item)->Draw();

	}

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
	/*if (console_window)
		DisplayConsole();*/
		
	//call config window function
	//if (configuration_window)
	//	DisplayConfig();

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



void ModuleEditor::HelpMarker(const char* desc)
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

void ModuleEditor::LoadConfig(JSON_Object *& config)
{

}

void ModuleEditor::ObjectEditor()
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

void ModuleEditor::AddFPS(float fps, float ms)
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

void ModuleEditor::Log(const char* fmt)
{
	if (console_window)
	{
		console_p->AddLog(fmt);
	}
}







void ModuleEditor::HelpScreen()
{
	if (ImGui::MenuItem("Gui Demo"))
	{
		demoWindow = (demoWindow == false) ? true : false;
	}

	if (ImGui::MenuItem("Documentation"))
	{
		//Add the link that guides users to the documentation
	}

	if (ImGui::MenuItem("Download latest"))
	{
		App->OpenWebsite("https://github.com/GerardClotet/Kuma-Engine/releases");
	}

	if (ImGui::MenuItem("Report a bug"))
	{
		App->OpenWebsite("https://github.com/GerardClotet/Kuma-Engine/issues");
	}
	if (ImGui::MenuItem("About"))
	{

	}
}

void ModuleEditor::ViewScreen()
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

void ModuleEditor::FileScreen()
{
	//TODO:/ WE NEED TO PUT HERE THE DIFFERENT OPTIONS OF THE MENU, AS OPEN, SAVE, QUIT...
	ImGui::SameLine();
	ImGui::TextDisabled("ESC");

}

void ModuleEditor::DisplayConfigDefault()
{
	ImGui::Begin("Load File", &config_default);

	ImGui::End();
}

