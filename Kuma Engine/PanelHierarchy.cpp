#include "PanelHierarchy.h"
#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "ModuleUI.h"



update_status PanelHierarchy::Draw()
{


	
	if (App->ui->hierarchy_window)
		DisplayHierarchy();
	return UPDATE_CONTINUE;
}

void PanelHierarchy::DisplayHierarchy()
{
	ImGui::Begin("Hierarchy", &App->ui->hierarchy_window);
	
	RecursiveTreeNode(App->scene_intro->root);

	ImGui::End();

}

void PanelHierarchy::RecursiveTreeNode(GameObject* parent)
{

	ImGuiTreeNodeFlags base_flags = /*ImGuiTreeNodeFlags_OpenOnArrow |*/ ImGuiTreeNodeFlags_DefaultOpen /*| ImGuiTreeNodeFlags_SpanAllAvailWidth // this flags ins't on*/;
	
	
	if (parent == App->scene_intro->selected_game_obj)
		base_flags = ImGuiTreeNodeFlags_Selected;

	if (parent->name.c_str() != nullptr && ImGui::TreeNodeEx(parent, base_flags, parent->name.c_str()))
	{
		if (ImGui::IsItemClicked())
			App->scene_intro->selected_game_obj = parent;

		
		ImGui::OpenPopupOnItemClick("test");

		ImGuiWindowFlags fl = ImGuiWindowFlags_MenuBar;
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginPopup("mem",fl);
		}
		
		std::vector<GameObject*>::const_iterator iter = parent->game_object_childs.begin(); //const_iterator just for reading
		while (iter != parent->game_object_childs.end())
		{
			//App->scene_intro->selected_game_obj = (*iter);
			RecursiveTreeNode((*iter));
			++iter;
		}
		ImGui::TreePop();

		
	}

	
}
