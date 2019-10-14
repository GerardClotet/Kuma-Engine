#include "PanelInspector.h"
#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "ModuleUI.h"

update_status PanelInspector::Draw()
{
	return UPDATE_CONTINUE;
}

void PanelInspector::DisplayInspector()
{
	ImGui::Begin("Object Inspector", &App->ui->inspector_window);


//	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	ImGui::End();
	
}

GameObject* PanelInspector::RecursiveTreeNode(GameObject* parent)
{
	return nullptr;
}
