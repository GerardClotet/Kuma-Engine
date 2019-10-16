#include "PanelInspector.h"
#include "GameObject.h"
#include "Component_Mesh.h"
#include "Component_Material.h"
#include "ModuleSceneIntro.h"
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

	if (ImGui::CollapsingHeader("Transform"))
	{
		if (App->scene_intro->selected_game_obj != nullptr)
		{
			static float pos[3] = { App->scene_intro->selected_game_obj->game_object_pos.x, 
									App->scene_intro->selected_game_obj->game_object_pos.y, 
									App->scene_intro->selected_game_obj->game_object_pos.z };
			ImGui::InputFloat3("Position", pos);


			static float rot[3] = { App->scene_intro->selected_game_obj->game_object_rot.x,
									App->scene_intro->selected_game_obj->game_object_rot.y,
									App->scene_intro->selected_game_obj->game_object_rot.z };
			ImGui::InputFloat3("Rotation", rot);

			static float scale[3] = { App->scene_intro->selected_game_obj->game_object_scale.x,
									App->scene_intro->selected_game_obj->game_object_scale.y,
									App->scene_intro->selected_game_obj->game_object_scale.z };
			ImGui::InputFloat3("Scale", scale);
		}
	}

	if (ImGui::CollapsingHeader("Mesh"))
	{
		if (App->scene_intro->selected_game_obj != nullptr)
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
			}
		}
	}


//	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	ImGui::End();
	
}

GameObject* PanelInspector::RecursiveTreeNode(GameObject* parent)
{
	return nullptr;
}
