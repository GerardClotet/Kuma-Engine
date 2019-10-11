#include "Globals.h"
#include "ModuleUI.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleHardware.h"
#include "PanelConfig.h"
#include "PanelConsole.h"
#include "PanelAbout.h"
#include <list>
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
	panel_list.push_back(about_p = new PanelAbout("About"));
	console_window = true;


	


	std::list<const char*>::iterator item = App->log_saves.begin();

	for (item; item != App->log_saves.end(); ++item)
	{
		const char* logs = (*item);

		LOG("%s", logs);
	}

	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	return true;

}

bool ModuleEditor::CleanUp()
{
	LOG("Unloading UI module");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	std::list<Panel*>::iterator item = panel_list.begin();
	while (item != panel_list.end())
	{
		//panel_list.erase(item);
		/*panel_list.remove(*item);*/
		++item;
	}
	panel_list.clear();
	console_p->Clear();
	//panel_list.clear();
	panel_list.clear();
	console_p = nullptr;
	config_p = nullptr;
	about_p = nullptr;
	return true;
}

update_status ModuleEditor::PostUpdate(float dt)
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

		if (ImGui::BeginMenu("GameObject"))
		{
			GameObjectScreen();
			ImGui::EndMenu();

		}

		if (ImGui::BeginMenu("Components"))
		{
			ComponentsScreen();
			ImGui::EndMenu();

		}

		if (ImGui::BeginMenu("Help"))
		{
			HelpScreen();

			ImGui::EndMenu();
		}


		ImGui::EndMainMenuBar();
	}
	for (std::list<Panel*>::iterator item = panel_list.begin(); item != panel_list.end(); ++item)
	{
		ret = (*item)->Draw();
	}
	//call demo function
	if (demoWindow)
		ImGui::ShowDemoWindow();

		

	if (show_obj_edit_window)
	{
		ObjectEditor();
	}

	ImGui::Render();

	
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return ret;
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

void ModuleEditor::LoadInputEvent(uint id, uint state)
{
	static char input_event[64];
	static const char* state_input[] = { "IDLE", "DOWN", "REPEAT", "UP" };
	if (id < 500)
	{
		sprintf_s(input_event, 64, "Keybr: %02u - %s\n", id, state_input[state]);
	}
	else
	{
		sprintf_s(input_event, 64, "Mouse: %02u - %s\n", (id - 500), state_input[state]);
	}
	if (config_p != nullptr)
	{
		config_p->AddInputToBuffer(input_event);
	}
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

void ModuleEditor::Log(const char* fmt,...)
{
	if ( console_p != nullptr &&console_window)
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
		App->OpenWebsite("https://github.com/GerardClotet/Kuma-Engine/blob/master/README.md");
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
		about_window = (about_window == false) ? true : false;
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

void ModuleEditor::GameObjectScreen()
{
	if (ImGui::MenuItem("Create Empty"))
	{

	}
	
	if (ImGui::BeginMenu("3D Objects"))
	{

		if (ImGui::MenuItem("Cube"))
		{
			App->importer->CreateCube({ 10,10,10 }, { 1.0f,0.0f,1.0f,1.0f });
		}

		if (ImGui::MenuItem("Sphere"))
		{
		}

		if (ImGui::MenuItem("Capsule"))
		{
		}

		if (ImGui::MenuItem("Cylinder"))
		{
		}

		if (ImGui::MenuItem("Plane"))
		{
		}

		ImGui::EndMenu();

	}

	if (ImGui::BeginMenu("Effects"))
	{
		if (ImGui::MenuItem("Particle System"))
		{
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Lights"))
	{

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Audio"))
	{

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("UI"))
	{

		ImGui::EndMenu();
	}
}

void ModuleEditor::ComponentsScreen()
{

}



