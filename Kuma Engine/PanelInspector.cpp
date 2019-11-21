#include "PanelInspector.h"
#include "GameObject.h"
#include "Component_Mesh.h"
#include "Component_Material.h"
#include "Component_Transform.h"
#include "Component_Camera.h"
#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "ModuleTexture.h"
#include "ModuleUI.h"
update_status PanelInspector::Draw()
{
	if (App->ui->inspector_window)
		DisplayInspector();


	return UPDATE_CONTINUE;
}

void PanelInspector::DisplayInspector()
{
	ImGui::Begin("Inspector", &App->ui->inspector_window);

	if (App->scene_intro->selected_game_obj != nullptr)
	{
		static char go_name[20];
		memcpy(go_name, App->scene_intro->selected_game_obj->name.data(), 20);
		if (ImGui::InputText("Name", go_name, 20, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			App->scene_intro->selected_game_obj->name = go_name;
		}

		if (ImGui::Checkbox("hide", &App->scene_intro->selected_game_obj->show))
		{
			LOG("hide");
		}

		ImGui::SameLine();
		if (ImGui::Button("delete", ImVec2(90, 20)))
		{
			if (App->scene_intro->selected_game_obj->hasComponent(GO_COMPONENT::CAMERA))
			{
				App->scene_intro->selected_game_obj->RemoveCameraFromist(App->scene_intro->selected_game_obj);
			}

			if (App->scene_intro->selected_game_obj->parent->type == OBJECT_TYPE::SUBPARENT)
			{
				App->scene_intro->root->RemoveSubChildGameObject(App->scene_intro->selected_game_obj);
			}

			else App->scene_intro->root->RemoveGameObject(App->scene_intro->selected_game_obj);

			
			
		}

		
		
	}
	
	if (ImGui::CollapsingHeader("Transform"))
	{
		if (App->scene_intro->selected_game_obj != nullptr && App->scene_intro->selected_game_obj->hasComponent(GO_COMPONENT::TRANSFORM))
		{
			App->scene_intro->selected_game_obj->transform->DisplayInspector();
		}
	}

	if (ImGui::CollapsingHeader("Mesh"))
	{
		if (App->scene_intro->selected_game_obj != nullptr && App->scene_intro->selected_game_obj->mesh != nullptr)
		{
			ImGui::Text("Mesh information");
			ImGui::Separator();
			ImGui::Text("%i num_vertex", App->scene_intro->selected_game_obj->mesh->num_vertex); 
			ImGui::Text("%i num_index", App->scene_intro->selected_game_obj->mesh->num_index);
			ImGui::Text("%i num_normals", App->scene_intro->selected_game_obj->mesh->num_normal);
			ImGui::Text("%i num_UVs", App->scene_intro->selected_game_obj->mesh->num_uvs);
			
			ImGui::Spacing();
			ImGui::Spacing();

			if (ImGui::Checkbox("VERTEX POINTS", &App->scene_intro->selected_game_obj->mesh->show_vertex)) {}
			if (ImGui::Checkbox("VERTEX NORMALS", &App->scene_intro->selected_game_obj->mesh->show_normals)) {}
			if (ImGui::Checkbox("FACE NORMALS", &App->scene_intro->selected_game_obj->mesh->show_face_normal)) {}
		}
	}

	if (ImGui::CollapsingHeader("Texture"))
	{
		if (App->scene_intro->selected_game_obj != nullptr)
		{
			if (App->scene_intro->selected_game_obj->material != nullptr && App->scene_intro->selected_game_obj->material->setTexture)
			{
				//if setTexture=true of the selected GO, do this
				ImGui::Text("%i Width %i Height", App->scene_intro->selected_game_obj->material->width, App->scene_intro->selected_game_obj->material->height);
				ImGui::Text("File Path: %s", App->scene_intro->selected_game_obj->material->file_path.c_str());

				texture = App->scene_intro->selected_game_obj->material->GetTexture();
				if(texture != nullptr && App->scene_intro->selected_game_obj->type != OBJECT_TYPE::SUBPARENT)
					ImGui::Image((ImTextureID)texture->id, ImVec2(200, 200),ImVec2(0,1),ImVec2(1,0)); 
				
				if ( App->scene_intro->selected_game_obj->type != OBJECT_TYPE::SUBPARENT && ImGui::Checkbox("GL_TEXTURE_2D", &GL_Texture_2D))
				{
					if (GL_Texture_2D && App->scene_intro->selected_game_obj != nullptr && App->scene_intro->selected_game_obj->material != nullptr)
					{
						glEnable(GL_TEXTURE_2D), LOG("Enabled Texture 2D");
						App->scene_intro->selected_game_obj->material->isTextureEnable = true;
					}

					else if (!GL_Texture_2D && App->scene_intro->selected_game_obj != nullptr && App->scene_intro->selected_game_obj->material != nullptr)
					{

						glDisable(GL_TEXTURE_2D), LOG("Disabled Texture 2D");

						App->scene_intro->selected_game_obj->material->isTextureEnable = false;
					}

				}
			}

	

		}

		if (App->scene_intro->selected_game_obj != nullptr)
		{
			if (ImGui::Checkbox("texture_checker", &App->scene_intro->selected_game_obj->texture_Checker))
			{
				if (App->scene_intro->selected_game_obj->material == nullptr)
					App->scene_intro->selected_game_obj->AddComponent(GO_COMPONENT::MATERIAL);
				App->scene_intro->selected_game_obj->texture_Checker ? App->scene_intro->selected_game_obj->material->SetDefaultTexture() : App->scene_intro->selected_game_obj->material->QuitDefautTexture();
			}
		}
		
	}

	if (ImGui::CollapsingHeader("Camera"))
	{
		if (App->scene_intro->camera_list.size() > 0)
		{
			static const char* item_current = (*App->scene_intro->camera_list.begin())->gameObject_Item->name.c_str();            // Here our selection is a single pointer stored outside the object.
			if (ImGui::BeginCombo("Display", item_current)) // The second parameter is the label previewed before opening the combo.
			{
				for (std::vector<Components*>::iterator item = App->scene_intro->camera_list.begin(); item != App->scene_intro->camera_list.end(); ++item)
				{
					bool is_selected = (item_current == (*item)->gameObject_Item->name.c_str());
					if (ImGui::Selectable((*item)->gameObject_Item->name.c_str(), is_selected))
					{
						item_current = (*item)->gameObject_Item->name.c_str();
						App->scene_intro->selected_camera_obj = (*item)->gameObject_Item;
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
				}
				ImGui::EndCombo();
			}
		}
		//-----------------------------------------------------------------------------

		if (App->scene_intro->selected_game_obj != nullptr && App->scene_intro->selected_game_obj->hasComponent(GO_COMPONENT::CAMERA))
		{
			App->scene_intro->selected_game_obj->camera->DisplayInspector();
		}
	}


	ImGui::End();
}


