#include "PanelScene.h"
#include "ImGui/examples/imgui_impl_opengl3.h"
#include "SDL/include/SDL_opengl.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include <string>

PanelScene::PanelScene(const char * name) : Panel(name)
{
	this->name = name;
}

update_status PanelScene::Draw()
{
	ImGui::Begin(name.c_str(), &enabled, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	size.x = App->window->GetScreenHeight();
	size.y = App->window->GetScreenWidth();

	ImGui::Image((ImTextureID)App->renderer3D->GetWinTexture(), ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();

	return UPDATE_CONTINUE;
}
