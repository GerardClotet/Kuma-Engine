#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleImporter.h"
#include "ModuleTexture.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "Component_Material.h"
#include "ModuleSceneIntro.h"

#include "ImGui/Impl/imgui_impl_sdl.h"

#include "ModuleUI.h"
#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	App->saveLog("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
			{
				keyboard[i] = KEY_DOWN;
				App->ui->LoadInputEvent(i, KEY_DOWN);
				App->ui->LoadInputEvent(i, KEY_REPEAT);
			}
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
			{
				keyboard[i] = KEY_UP;
				App->ui->LoadInputEvent(i, KEY_UP);
			}
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_wheel = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if (mouse_buttons[i] == KEY_IDLE)
			{
				mouse_buttons[i] = KEY_DOWN;
				App->ui->LoadInputEvent(500 + i, KEY_DOWN);
				App->ui->LoadInputEvent(500 + i, KEY_REPEAT);
			}
			else
				mouse_buttons[i] = KEY_REPEAT;
			
		}
		else
		{
			if (mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
			{
			
				mouse_buttons[i] = KEY_UP;
				App->ui->LoadInputEvent(500 + i, KEY_UP);
			}
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_wheel = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;


			case SDL_QUIT:
			quit = true;
			break;

			case SDL_DROPFILE:
				ExtensionFileDecider(e.drop.file);
				SDL_free(e.drop.file);
				break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
		}
	}


	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

void ModuleInput::ExtensionFileDecider(const char* file)
{
	std::string file_decider = file;
	std::string extension = file_decider.substr(file_decider.find_last_of(".") + 1);
	if (extension == "FBX" || extension == "fbx")
	{
		App->importer->LoadGeometry(file_decider.c_str());
	}
	else if (extension == "PNG" || extension == "png" || extension == "jpg")
	{
		/*for (std::list<FBX*>::iterator item_fbx = App->importer->fbx_list.begin(); item_fbx != App->importer->fbx_list.end(); ++item_fbx)
		{
			for (std::list<Mesh*>::iterator item_mesh = (*item_fbx)->mesh_list_fbx.begin(); item_mesh != (*item_fbx)->mesh_list_fbx.end(); ++item_mesh)
			{
				(*item_mesh)->texture = App->texture->LoadTexture(file_decider.c_str());
			}
		}*/
		for (std::vector<GameObject*>::iterator item = App->scene_intro->gameObject_list.begin(); item != App->scene_intro->gameObject_list.end(); ++item)
		{
			if ((*item)->name == "prove")
			{
				(*item)->AddComponent(GO_COMPONENT::MATERIAL);
				for (std::vector<Components*>::iterator item_com = (*item)->components.begin(); item_com != (*item)->components.end(); ++item_com)
				{
					if ((*item_com)->comp_type == GO_COMPONENT::MATERIAL)
					{
						//(*item)->material->texture = App->texture->LoadTexture(file_decider.c_str());
						(*item)->material->ReadTexture(file_decider.c_str());
					}
				}
			}
		}
	}

	
}
