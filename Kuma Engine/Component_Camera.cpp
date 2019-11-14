#include "Component_Camera.h"
#include "Application.h"
#include "GameObject.h"
#include "ModuleWindow.h"
#include "Component_Transform.h"
#include "ImGui/imgui.h"
Component_Camera::Component_Camera(GameObject* game_object)
{
	this->gameObject_Item = game_object;
	name = "camera";
	SetAspectRatio(16, 9);
	
	float a  = frustum.horizontalFov;
	LOG("%f", a);
	
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = 1.0f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = 60.0f * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.3f);

	far_plane = frustum.farPlaneDistance;
	near_plane = frustum.nearPlaneDistance;
	vertical_fov = frustum.verticalFov;
	horizontal_fov = frustum.horizontalFov;

	color_camera_bg = Color(0.5f, 0.5f, 0.5f, 1.0f);

	ReloadFrustrum();
}

Component_Camera::~Component_Camera()
{
}

void Component_Camera::ReloadFrustrum()
{
	
	frustum.GetCornerPoints(points);
	
}

void Component_Camera::DrawFrustum()
{
	ReloadFrustrum();


	glLineWidth(3.0);
	glColor3f(1, .54, 0);
	glBegin(GL_LINES);

	glVertex3f(points[0].x, points[0].y, points[0].z);
	glVertex3f(points[1].x, points[1].y, points[1].z);

	glVertex3f(points[0].x, points[0].y, points[0].z);
	glVertex3f(points[4].x, points[4].y, points[4].z);

	glVertex3f(points[4].x, points[4].y, points[4].z);
	glVertex3f(points[5].x, points[5].y, points[5].z);

	glVertex3f(points[0].x, points[0].y, points[0].z);
	glVertex3f(points[2].x, points[2].y, points[2].z);

	glVertex3f(points[2].x, points[2].y, points[2].z);
	glVertex3f(points[3].x, points[3].y, points[3].z);

	glVertex3f(points[1].x, points[1].y, points[1].z);
	glVertex3f(points[3].x, points[3].y, points[3].z);

	glVertex3f(points[1].x, points[1].y, points[1].z);
	glVertex3f(points[5].x, points[5].y, points[5].z);

	glVertex3f(points[4].x, points[4].y, points[4].z);
	glVertex3f(points[6].x, points[6].y, points[6].z);

	glVertex3f(points[2].x, points[2].y, points[2].z);
	glVertex3f(points[6].x, points[6].y, points[6].z);

	glVertex3f(points[6].x, points[6].y, points[6].z);
	glVertex3f(points[7].x, points[7].y, points[7].z);

	glVertex3f(points[5].x, points[5].y, points[5].z);
	glVertex3f(points[7].x, points[7].y, points[7].z);

	glVertex3f(points[3].x, points[3].y, points[3].z);
	glVertex3f(points[7].x, points[7].y, points[7].z);

	glEnd();
	glLineWidth(1.0);
}

void Component_Camera::UpdateTransformFrustum()
{
	gameObject_Item->transform->GetGlobalMatrix();
	frustum.pos = gameObject_Item->transform->GetGlobalPosition();

	frustum.front = gameObject_Item->transform->GetGlobalRotation() * float3::unitZ;
	frustum.up = gameObject_Item->transform->GetGlobalRotation() * float3::unitY;;
}

void Component_Camera::DisplayInspector()
{
	
	if (ImGui::DragFloat("Near Plane", &near_plane, 1, 0.1f, far_plane - 0.1f, "%.1f"))
	{
		frustum.nearPlaneDistance = near_plane;
	}

	ImGui::Spacing();


	if (ImGui::DragFloat("Far Plane", &far_plane, 1, near_plane + 0.1f, 500, "%.1f"))
	{
		frustum.farPlaneDistance = far_plane;
	}

	ImGui::Spacing();
	ImGui::Spacing();

	

	if (ImGui::DragFloat("FOV Horizontal", &horizontal_fov, 1, 1, 163, "%.1f"))
	{
		frustum.horizontalFov = horizontal_fov * DEGTORAD;
		SetAspectRatio(16, 9, true);
		vertical_fov = frustum.verticalFov * RADTODEG;
	}
	
	

	if (ImGui::DragFloat("FOV Vertical", &vertical_fov, 1, 1, 150, "%.1f"))
	{
		frustum.verticalFov = vertical_fov * DEGTORAD;
		SetAspectRatio(16, 9);
		horizontal_fov = frustum.horizontalFov * RADTODEG;
	}
	


}

void Component_Camera::Look(const float3 & position)
{
	float3 direction = position - frustum.pos;

	float3x3 matrix = float3x3::LookAt(frustum.front, direction.Normalized(), frustum.up, float3::unitY);

	frustum.front = matrix.MulDir(frustum.front).Normalized();
	frustum.up = matrix.MulDir(frustum.up).Normalized();
}

float3 Component_Camera::GetCameraPosition() const
{
	return frustum.pos;
}

float * Component_Camera::GetViewMatrix()
{
	return (float*)static_cast<float4x4>(frustum.ViewMatrix()).Transposed().v;
}

float * Component_Camera::GetProjectionMatrix() const
{
	return (float*)frustum.ProjectionMatrix().Transposed().v;
}

void Component_Camera::SetAspectRatio(int width_ratio, int height_ratio, bool type)
{
	if (!type)
	{
		frustum.horizontalFov = (2.f * atanf(tanf(frustum.verticalFov * 0.5f) * ((float)width_ratio / (float)height_ratio)));
	}
	else
	{
		frustum.verticalFov = (2.f * atanf(tanf(frustum.horizontalFov * 0.5f) * ((float)height_ratio) / (float)width_ratio));
		
	}
	
}

bool Component_Camera::Update()
{
	UpdateTransformFrustum();
	DrawFrustum();
	return true;
}
