#include "Globals.h"
#include "ModuleUI.h"
#include "Application.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

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

	}
	if (ImGui::CollapsingHeader("Window"))
	{

	}
	if (ImGui::CollapsingHeader("File System"))
	{

	}
	if (ImGui::CollapsingHeader("Input"))
	{

	}
	if (ImGui::CollapsingHeader("Hardware"))
	{

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

