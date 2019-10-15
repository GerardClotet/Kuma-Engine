#include "PanelInspector.h"
#include "GameObject.h"
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
		static float vec4a[4] = { 0.00f, 0.00f, 0.00f, 0.00f };
		ImGui::InputFloat3("Position",vec4a);

		static float vec4b[4] = { 1.00f, 1.00f, 1.00f, 0.00f };
		ImGui::InputFloat3("Rotation", vec4b);

		static float vec4c[4] = { 1.00f, 1.00f, 1.00f, 0.00f };
		ImGui::InputFloat3("Scale", vec4c);
	}

	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::Text("Mesh information");
		ImGui::Separator();
		ImGui::Text("%i num_vertex"); //REPLACE THIS %i WITH THE REAL NUMBERS WITH THE SELECTED GO
		ImGui::Text("%i num_index");
		ImGui::Text("%i num_normals");
		ImGui::Text("%i num_UVs");

		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Checkbox("VERTEX POINTS", &App->ui->show_vertex)) {}
		if (ImGui::Checkbox("VERTEX NORMALS", &App->ui->show_normals)) {}
		if (ImGui::Checkbox("FACE NORMALS", &App->ui->show_face_normal)) {}
	}

	if (ImGui::CollapsingHeader("Texture"))
	{
		//if setTexture=true of the selected GO, do this
		ImGui::Text("%i Width %i Height");
		ImGui::Text("File Path: %s"); //path. We have a variable named "file_path" you can get through the pointer material

	}


//	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	ImGui::End();
	
}

GameObject* PanelInspector::RecursiveTreeNode(GameObject* parent)
{
	return nullptr;
}
