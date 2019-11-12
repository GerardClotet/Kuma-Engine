#include "Component_Camera.h"
#include "Application.h"
#include "GameObject.h"
#include "ModuleWindow.h"
#include "Component_Transform.h"
Component_Camera::Component_Camera(GameObject* game_object)
{
	this->gameObject_Item = game_object;
	name = "camera";
	SetAspectRatio();
	
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

	float aspect = frustum.AspectRatio();
	//gameObject_Item->transform->local_transformation = frustum.ProjectionMatrix();
	//
	//float4x4 vie_mat = frustum.ViewMatrix();

	//frustum.WorldRight();
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

	frustum.front = gameObject_Item->transform->GetGlobalRotation() * float3 (0,0,1);
	frustum.up = gameObject_Item->transform->GetGlobalRotation() * float3(0, 1, 0);
}

void Component_Camera::SetAspectRatio()
{
	height = App->window->GetScreenHeight();
	width = App->window->GetScreenWidth();

	aspect_ratio = height / width;
	
}

bool Component_Camera::Update()
{
	UpdateTransformFrustum();
	DrawFrustum();
	return true;
}
