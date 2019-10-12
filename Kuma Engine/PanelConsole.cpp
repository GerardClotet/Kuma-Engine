#include "PanelConsole.h"

#include "Panel.h"
#include "Application.h"
#include "ModuleUI.h"
update_status PanelConsole::Draw()
{
	if (App->ui->console_window)DisplayConsole();
	return UPDATE_CONTINUE;
}


void PanelConsole::DisplayConsole()
{
	ImGui::Begin("Console", &App->ui->console_window);

	if (ImGui::Button("Clear")) Clear();
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	Filter.Draw("Filter", -100.0f);
	ImGui::Separator();
	ImGui::BeginChild("scrolling");
	if (copy) ImGui::LogToClipboard();


		ImGui::TextUnformatted(Buf.begin());
	

	if (ScrollToBottom)
		ImGui::SetScrollHere(1.0f);
	ScrollToBottom = false;
	ImGui::EndChild();
	ImGui::End();
	
}

void PanelConsole::Clear()
{
	Buf.clear(); 
}

void PanelConsole::AddLog(const char* fmt,...)
{
	Buf.appendf(fmt);
	ScrollToBottom = true;
}


