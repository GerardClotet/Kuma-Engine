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


//	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	ImGui::End();
	
}

GameObject* PanelInspector::RecursiveTreeNode(GameObject* parent)
{
	return nullptr;
}
