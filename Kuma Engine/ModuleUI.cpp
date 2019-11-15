#include "Globals.h"
#include "ModuleUI.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleHardware.h"
#include "PanelConfig.h"
#include "PanelConsole.h"
#include "PanelAbout.h"
#include "PanelInspector.h"
#include "PanelHierarchy.h"
#include "PanelFile.h"
#include "ModuleRenderer3D.h"
#include "ModuleFileSystem.h"
#include "ImGui/imgui.h"
#include "ImGui/imconfig.h"
#include "ImGui/examples/imgui_impl_opengl3.h"
#include "ImGui/examples/imgui_impl_sdl.h"
#include <list>
#include <algorithm>
#include "GameObject.h"
#include "Components.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	fps_log.resize(100);
	ms_log.resize(100);
	selected_file[0] = '\0';
	
}

ModuleEditor::~ModuleEditor()
{}


bool ModuleEditor::Start()
{

	
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;//enable docking   
	io.ConfigDockingWithShift = false; // dock without shift
	io.ConfigWindowsResizeFromEdges = true;
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;

	  // Setup Dear ImGui style
	ImGui::StyleColorsEdited();
	
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();
	panel_list.push_back(config_p		= new PanelConfig("Configuration"));
	panel_list.push_back(console_p		= new PanelConsole("Console"));
	panel_list.push_back(about_p		= new PanelAbout("About"));
	panel_list.push_back(inspector_p	= new PanelInspector("Inspector"));
	panel_list.push_back(hierarchy_p	= new PanelHierarchy("Hierarchy"));
	panel_list.push_back(file_p			= new PanelFile("File"));
	console_window = true;
	hierarchy_window = true;
	inspector_window = true;
	


	

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

update_status ModuleEditor::Update(float dt)
{
	/*if (App->scene_intro->selected_game_obj !=nullptr)
		inspector_window = true;*/
	
	
	return UPDATE_CONTINUE;
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
	}//end menu bar --> maybe a func for this?

	BackgroundDockSpace();

	for (std::list<Panel*>::iterator item = panel_list.begin(); item != panel_list.end(); ++item)
	{
		ret = (*item)->Draw();
	}
	//call demo function
	if (demoWindow)
		ImGui::ShowDemoWindow();//ImGui::ShowDemoWindow();

		

	if (show_obj_edit_window)
	{
		ObjectEditor();
	}

	ImGui::Render();

	
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return ret;
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

void ModuleEditor::BackgroundDockSpace()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::SetNextWindowBgAlpha(0.0f);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Invisible Background",0, window_flags);
	ImGui::PopStyleVar(3);


	
	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

	ImGui::End();
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
	/*ImGui::SameLine();
	ImGui::TextDisabled("1");*/


	if (ImGui::MenuItem("Configuration"))
	{
		configuration_window = (configuration_window == false) ? true : false;
		//this is the configuration
	}
	//ImGui::SameLine();
		//ImGui::TextDisabled("4");

	if (ImGui::MenuItem("Hierarchy"))
	{
		hierarchy_window = (hierarchy_window == false) ? true : false;
	}
}

void ModuleEditor::FileScreen()
{
	//TODO:/ WE NEED TO PUT HERE THE DIFFERENT OPTIONS OF THE MENU, AS OPEN, SAVE, QUIT...
	ImGui::SameLine();
	ImGui::TextDisabled("ESC");

	if (ImGui::MenuItem("Load File"))
	{
		file_window = (file_window == false) ? true : false;
	}
	if (ImGui::MenuItem("Save File"))
	{

	}

}

void ModuleEditor::GameObjectScreen()
{
	if (ImGui::MenuItem("Inspector"))
	{
		inspector_window = (inspector_window == false) ? true : false;
	}
	if (ImGui::MenuItem("Create Empty GameObject"))
	{
		GameObject* go = App->scene_intro->CreateGameObject(nullptr, OBJECT_TYPE::NONE, "Empty GameObject");
	}
	if (ImGui::MenuItem("Create Camera"))
	{
		GameObject* go = App->scene_intro->CreateGameObject(nullptr, OBJECT_TYPE::NONE, "Camera");
		go->AddComponent(GO_COMPONENT::TRANSFORM, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f,0.0f });
		go->AddComponent(GO_COMPONENT::CAMERA);
	}
	
	if (ImGui::BeginMenu("3D Objects"))
	{

		if (ImGui::MenuItem("Cube"))
		{
			GameObject* go = nullptr;
			go = App->scene_intro->CreateGameObject(go, OBJECT_TYPE::CUBE, "cube");
			go->AddComponent(GO_COMPONENT::MESH);
			go->AddComponent(GO_COMPONENT::TRANSFORM, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f,0.0f });
			App->scene_intro->selected_game_obj = go;
			LOG("----Created Cube----");
		}

		if (ImGui::MenuItem("Sphere"))
		{
			GameObject* go = nullptr;
			go = App->scene_intro->CreateGameObject(go, OBJECT_TYPE::SPHERE, "sphere");
			go->AddComponent(GO_COMPONENT::MESH);
			go->AddComponent(GO_COMPONENT::TRANSFORM, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f,0.0f });
			App->scene_intro->selected_game_obj = go;
			LOG("----Created Sphere----");

		}

		if (ImGui::MenuItem("Cone"))
		{

			GameObject* go = nullptr;
			go = App->scene_intro->CreateGameObject(go, OBJECT_TYPE::CONE, "cone");
			go->AddComponent(GO_COMPONENT::MESH);
			go->AddComponent(GO_COMPONENT::TRANSFORM, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f,0.0f });
			App->scene_intro->selected_game_obj = go;
			LOG("----Created Cone----");
		}

		if (ImGui::MenuItem("Cylinder"))
		{
			GameObject* go = nullptr;
			go = App->scene_intro->CreateGameObject(go, OBJECT_TYPE::CYLINDER, "cylinder");
			go->AddComponent(GO_COMPONENT::MESH);
			go->AddComponent(GO_COMPONENT::TRANSFORM, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f,0.0f });
			App->scene_intro->selected_game_obj = go;
			LOG("----Created Cylinder----");
		}

		if (ImGui::MenuItem("Dodecahedron"))
		{
			GameObject* go = nullptr;
			go = App->scene_intro->CreateGameObject(go, OBJECT_TYPE::DODECAHEDRON, "dodecahedron");
			go->AddComponent(GO_COMPONENT::MESH);
			go->AddComponent(GO_COMPONENT::TRANSFORM, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f,0.0f });
			App->scene_intro->selected_game_obj = go;
			LOG("----Created Dodecahedron----");
		}
		if (ImGui::MenuItem("Plane"))
		{
			/*GameObject* go = nullptr;
			go = App->scene_intro->CreateGameObject(go, OBJECT_TYPE::PLANE, "plane");
			go->AddComponent(GO_COMPONENT::MESH);
			go->AddComponent(GO_COMPONENT::TRANSFORM);
			App->scene_intro->selected_game_obj = go;
			LOG("----Created Plane----");*/
			LOG("plane WIP sorry :(");

		}
		if (ImGui::MenuItem("Torus"))
		{
			GameObject* go = nullptr;
			go = App->scene_intro->CreateGameObject(go, OBJECT_TYPE::TORUS, "torus");
			go->AddComponent(GO_COMPONENT::MESH);
			go->AddComponent(GO_COMPONENT::TRANSFORM, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f,0.0f });
			App->scene_intro->selected_game_obj = go;
			LOG("----Created Torus----");
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
	if (ImGui::MenuItem("Transform"))
	{
		if (App->scene_intro->selected_game_obj != nullptr && !App->scene_intro->selected_game_obj->hasComponent(GO_COMPONENT::TRANSFORM))
		{
			if (App->scene_intro->selected_game_obj->type == OBJECT_TYPE::SUBPARENT)
			{
				std::vector<GameObject*>::iterator iter = App->scene_intro->selected_game_obj->game_object_childs.begin();
				for (iter; iter != App->scene_intro->selected_game_obj->game_object_childs.end(); ++iter)
				{
					if (!(*iter)->hasComponent(GO_COMPONENT::TRANSFORM))
						(*iter)->AddComponent(GO_COMPONENT::TRANSFORM);
				}
			}
			else
			{
				App->scene_intro->selected_game_obj->AddComponent(GO_COMPONENT::TRANSFORM);
			}
		}
		
	}
	if (ImGui::MenuItem("Mesh"))
	{
		if (App->scene_intro->selected_game_obj != nullptr && !App->scene_intro->selected_game_obj->hasComponent(GO_COMPONENT::MESH))
		{
			if (App->scene_intro->selected_game_obj->type == OBJECT_TYPE::SUBPARENT)
			{
				std::vector<GameObject*>::iterator iter = App->scene_intro->selected_game_obj->game_object_childs.begin();
				for (iter; iter != App->scene_intro->selected_game_obj->game_object_childs.end(); ++iter)
				{
					if (!(*iter)->hasComponent(GO_COMPONENT::MESH))
						(*iter)->AddComponent(GO_COMPONENT::MESH);
				}
			}
			else
			{
				App->scene_intro->selected_game_obj->AddComponent(GO_COMPONENT::MESH);
			}
		}
		
	}

	if (ImGui::MenuItem("Material"))
	{
		if (App->scene_intro->selected_game_obj != nullptr && !App->scene_intro->selected_game_obj->hasComponent(GO_COMPONENT::MATERIAL))
		{
			if (App->scene_intro->selected_game_obj->type == OBJECT_TYPE::SUBPARENT)
			{
				std::vector<GameObject*>::iterator iter = App->scene_intro->selected_game_obj->game_object_childs.begin();
				for (iter; iter != App->scene_intro->selected_game_obj->game_object_childs.end(); ++iter)
				{
					if (!(*iter)->hasComponent(GO_COMPONENT::MATERIAL))
						(*iter)->AddComponent(GO_COMPONENT::MATERIAL);
				}
			}
			else
			{
				App->scene_intro->selected_game_obj->AddComponent(GO_COMPONENT::MATERIAL);
			}

		}
	}
	
}

void ModuleEditor::LoadFile(const char * filter_extension, const char * from_dir)
{
	ImGui::Begin("Load File", &App->ui->file_window);

	ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
	ImGui::BeginChild("File Browser", ImVec2(0, 500), true);
	DrawDirectoryTree(from_dir, filter_extension);
	ImGui::EndChild();
	ImGui::PopStyleVar();
	ImGui::End();
}

void ModuleEditor::DrawDirectoryTree(const char * directory, const char * filter_extension)
{

	std::vector<std::string> files_list;
	std::vector<std::string> dirs_list;

	//If the directory is empty
	std::string dir((directory) ? directory : "");
	dir += "/";

	//Add the files and directories to the lists
	App->fs->DiscoverFiles(dir.c_str(), files_list, dirs_list);

	for (std::vector<std::string>::const_iterator it = dirs_list.begin(); it != dirs_list.end(); ++it)
	{
		if (ImGui::TreeNodeEx((dir + (*it)).c_str(), 0, "%s/", (*it).c_str()))
		{
			DrawDirectoryTree((dir + (*it)).c_str(), filter_extension);
			ImGui::TreePop();
		}
	}

	std::sort(files_list.begin(), files_list.end());

	for (std::vector<std::string>::const_iterator it = files_list.begin(); it != files_list.end(); ++it)
	{
		const std::string& file_str = *it;

		bool correctExtension = true;

		//Apply the filter. If the extensions doesn't match with the filter extension, it wont be shown
		if (filter_extension && file_str.substr(file_str.find_last_of(".") + 1) != filter_extension)
			correctExtension = false;

		if (correctExtension && ImGui::TreeNodeEx(file_str.c_str(), ImGuiTreeNodeFlags_Leaf))
		{
			//If the file is clicked add a selected file
			//TODO :/Call the LoadScene with the selected_file
			if (ImGui::IsItemClicked()) {
				sprintf_s(selected_file, FILE_MAX, "%s%s", dir.c_str(), file_str.c_str());

				//Close the window if a file is double clicked
				if (ImGui::IsMouseDoubleClicked(0))
					file_window = false;
			}

			ImGui::TreePop();
		}
	}

}



