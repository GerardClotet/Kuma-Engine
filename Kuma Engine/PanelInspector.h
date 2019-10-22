#ifndef _PANEL_INSPECTOR_H_
#define _PANEL_INSPECTOR_H_

#include <vector>

#include "Panel.h"
#include "Application.h"

#include "ImGui/imgui.h"
struct TexData;
class GameObject;
class PanelInspector : public Panel
{
public:

	PanelInspector() {};
	PanelInspector(const char* name) : Panel(name) { App->saveLog("Inspector Panel Created"); };
	~PanelInspector() {};

	update_status Draw();


	void DisplayInspector();
	TexData* texture;

public:
	bool GL_Texture_2D = true;

	/*static  ImVec2 ImRotate(const ImVec2& v, float cos_a, float sin_a);
	static  inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs);
	void ImageRotated(ImTextureID tex_id, ImVec2 center, ImVec2 size, float angle);
*/
};
#endif // !_PANEL_INSPECTOR_H_
