#include "PanelInspector.h"
#include "GameObject.h"
#include "Component_Mesh.h"
#include "Component_Material.h"
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
			static float pos[3] = { App->scene_intro->selected_game_obj->game_object_pos.x, 
									App->scene_intro->selected_game_obj->game_object_pos.y, 
									App->scene_intro->selected_game_obj->game_object_pos.z };
			ImGui::InputFloat3("Position", pos);
			if (pos[0] != App->scene_intro->selected_game_obj->game_object_pos.x)
				App->scene_intro->selected_game_obj->mesh->TranslateMesh(pos);


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



//	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	ImGui::End();
}


//static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
//{
//	return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
//}
//static inline ImVec2 ImRotate(const ImVec2& v, float cos_a, float sin_a)
//{
//	return ImVec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a);
//}
//void ImageRotated(ImTextureID tex_id, ImVec2 center, ImVec2 size, float angle)
//{
//	ImDrawList* draw_list = ImGui::GetWindowDrawList();
//
//	float cos_a = cosf(angle);
//	float sin_a = sinf(angle);
//	ImVec2 pos[4] =
//	{
//		center + ImRotate(ImVec2(-size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
//		center + ImRotate(ImVec2(+size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
//		center + ImRotate(ImVec2(+size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a),
//		center + ImRotate(ImVec2(-size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a)
//	};
//	ImVec2 uvs[4] =
//	{
//		ImVec2(0.0f, 0.0f),
//		ImVec2(1.0f, 0.0f),
//		ImVec2(1.0f, 1.0f),
//		ImVec2(0.0f, 1.0f)
//	};
//
//	draw_list->AddImageQuad(tex_id, pos[0], pos[1], pos[2], pos[3], uvs[0], uvs[1], uvs[2], uvs[3], IM_COL32_WHITE);
//}

//ImVec2 PanelInspector::ImRotate(const ImVec2& v, float cos_a, float sin_a)
//{
//	return ImVec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a);
//}
//
// inline ImVec2 PanelInspector::operator+(const ImVec2& lhs, const ImVec2& rhs)
//{
//	return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
//}
//
//void PanelInspector::ImageRotated(ImTextureID tex_id, ImVec2 center, ImVec2 size, float angle)
//{
//	ImDrawList* draw_list = ImGui::GetWindowDrawList();
//
//	float cos_a = cosf(angle);
//	float sin_a = sinf(angle);
//	ImVec2 pos[4] =
//	{
//	ImRotate(ImVec2(-size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
//	 ImRotate(ImVec2(+size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
//	 ImRotate(ImVec2(+size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a),
//	ImRotate(ImVec2(-size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a)
//	};
//	ImVec2 uvs[4] =
//	{
//		ImVec2(0.0f, 0.0f),
//		ImVec2(1.0f, 0.0f),
//		ImVec2(1.0f, 1.0f),
//		ImVec2(0.0f, 1.0f)
//	};
//
//	draw_list->AddImageQuad(tex_id, pos[0], pos[1], pos[2], pos[3], uvs[0], uvs[1], uvs[2], uvs[3], IM_COL32_WHITE);
//}

