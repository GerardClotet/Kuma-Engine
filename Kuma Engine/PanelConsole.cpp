#include "PanelConsole.h"
#include "ImGui/imgui.h"
#include "Panel.h"
#include "Application.h"
update_status PanelConsole::Draw()
{




	if (App->ui->console_window)DisplayConsole();
	return UPDATE_CONTINUE;
}


void PanelConsole::DisplayConsole()
{
//	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	ImGui::Begin("Console", &App->ui->console_window);

	if (ImGui::Button("Clear")) Clear();
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	Filter.Draw("Filter", -100.0f);
	ImGui::Separator();
	ImGui::BeginChild("scrolling");
	//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
	if (copy) ImGui::LogToClipboard();


	//if (Filter.IsActive())
	//{
	//	const char* buf_begin = Buf.begin();
	//	const char* line = buf_begin;
	//	/*for (int line_no = 0; line != NULL; line_no++)
	//	{
	//		const char* line_end = (line_no < LineOffsets.Size) ? buf_begin + LineOffsets[line_no] : NULL;
	//		if (Filter.PassFilter(line, line_end))
	//			ImGui::TextUnformatted(line, line_end);
	//		line = line_end && line_end[1] ? line_end + 1 : NULL;
	//	}*/
	//}
	//else
	//{
		ImGui::TextUnformatted(Buf.begin());
	//}

	if (ScrollToBottom)
		ImGui::SetScrollHere(1.0f);
	ScrollToBottom = false;
	//ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::End();
	
}

void PanelConsole::Clear()
{
	Buf.clear(); LineOffsets.clear();
}

void PanelConsole::AddLog(const char* fmt)
{
	Buf.appendf(fmt);
	ScrollToBottom = true;
}


