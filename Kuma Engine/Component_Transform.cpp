#include "Component_Transform.h"
#include "Application.h"
#include "GameObject.h"
#include "ModuleUI.h"
#include "ImGui/imgui.h"
#include "Component_Mesh.h"
Component_Transform::Component_Transform(GameObject * obj, float3 pos, float3 scale, Quat rot)
{
	name = "transform";
	this->gameObject_Item = obj;
	this->local_position = pos;
	this->local_scale = scale;
	this->local_rotation = rot;

	//pass the quaternion to euler angles
	this->euler_rotation = this->local_rotation.ToEulerXYZ();
	//pass the euler angles from radiants to degrees
	this->euler_rotation.x = RadToDeg(this->euler_rotation.x);
	this->euler_rotation.y = RadToDeg(this->euler_rotation.y);
	this->euler_rotation.z = RadToDeg(this->euler_rotation.z);

	//initialize local transformation
	local_transformation = float4x4::FromTRS(this->local_position, this->local_rotation, this->local_scale);

	//Check if this game object has a parent. If it has, it's local transformation depends on it's parent global transform
	if (gameObject_Item->parent != nullptr)
	{
		if (gameObject_Item->parent->hasComponent(GO_COMPONENT::TRANSFORM))
			global_transformation = gameObject_Item->parent->transform->global_transformation * local_transformation;
		
		//If the parent exists but it has no Transform Component.
		else
			global_transformation = local_transformation;
		
		
	}
	//If the game object has no parent
	else
		global_transformation = local_transformation;
	

	comp_type = GO_COMPONENT::TRANSFORM;
}

Component_Transform::~Component_Transform()
{
	LOG("deleted transform");
	gameObject_Item = nullptr;
	
}

bool Component_Transform::Update()
{
	
	return true;
}

bool Component_Transform::Enable()
{
	return true;
}

bool Component_Transform::Disable()
{
	return true;
}

void Component_Transform::SetLocalPosition(const float x, const float y, const float z)
{
	local_position.x = x;
	local_position.y = y;
	local_position.z = z;

	RecalculateTransformMatrix();
}

float3& Component_Transform::GetLocalPosition()
{
	return local_position;
}

const float3 Component_Transform::GetGlobalPosition()const
{
	float3 pos, scale;
	Quat rot;

	global_transformation.Decompose(pos, rot, scale);

	return pos;

}

void Component_Transform::SetLocalScale(const float x, const float y, const float z)
{
	local_scale.x = x;
	local_scale.y = y;
	local_scale.z = z;

	RecalculateTransformMatrix();
}

const float3 Component_Transform::GetLocalScale()const
{

	return local_scale;
}

const float3 Component_Transform::GetGlobalScale()const
{
	float3 pos, scale;
	Quat rot;

	global_transformation.Decompose(pos, rot, scale);

	return scale;
}

void Component_Transform::SetLocalPosToSingleAxis(const float pos, int axis)
{
	if (axis == 0)
		local_position.x = pos;
	else if (axis == 1)
		local_position.y = pos;
	else if (axis == 2)
		local_position.z = pos;
}

void Component_Transform::SetLocalRotToSingleAxis(const float rot, int axis)
{
	if (axis == 0)
		local_rotation.x = rot;
	else if (axis == 1)
		local_rotation.y = rot;
	else if (axis == 2)
		local_rotation.z = rot;
	else if (axis == 3)
		local_rotation.w = rot;
}

void Component_Transform::SetLocalScaleToSingleAxis(const float scale, int axis)
{
	if (axis == 0)
		local_rotation.x = scale;
	else if (axis == 1)
		local_rotation.y = scale;
	else if (axis == 2)
		local_rotation.z = scale;
}

void Component_Transform::SetLocalRotation( float x,  float y,  float z,  float angle)
{
	local_rotation.x = x;
	local_rotation.y = y;
	local_rotation.z = z;
	local_rotation.w = angle;

	//pass the quaternion to euler angles
	euler_rotation = local_rotation.ToEulerXYZ();
	//pass the euler angles from radiants to degrees
	euler_rotation.x = RadToDeg(euler_rotation.x);
	euler_rotation.y = RadToDeg(euler_rotation.y);
	euler_rotation.z = RadToDeg(euler_rotation.z);

	RecalculateTransformMatrix();
}

const Quat Component_Transform::GetLocalRotation()const
{
	return local_rotation;
}

const Quat Component_Transform::GetGlobalRotation()const
{
	float3 pos, scale;
	Quat rot;

	global_transformation.Decompose(pos, rot, scale);

	return rot;
}

void Component_Transform::RecalculateTransformMatrix()
{
	//Get the local transformation
	local_transformation = float4x4::FromTRS(local_position, local_rotation, local_scale);

	//If the game object is null, avoid this
	if (gameObject_Item == nullptr)
		return;

	//Check if the gameobject has a parent
	if (gameObject_Item->parent != nullptr)
	{
		if (gameObject_Item->parent->hasComponent(GO_COMPONENT::TRANSFORM))
			global_transformation = gameObject_Item->parent->transform->global_transformation * local_transformation;

		//If the parent exists but it has no Transform Component.
		else
			global_transformation = local_transformation;
	}
	//If the game object has no parent
	else
		global_transformation = local_transformation;


	//Iterate the childrens if it has
	std::vector<GameObject*>::iterator item = gameObject_Item->game_object_childs.begin();
	for (; item != gameObject_Item->game_object_childs.end(); ++item)
	{
		if ((*item) != nullptr)
		{
			if ((*item)->hasComponent(GO_COMPONENT::TRANSFORM))
			{
				(*item)->transform->RecalculateTransformMatrix();
			

			}
		}
	}
	gameObject_Item->TransformBBox();


}

float4x4 Component_Transform::GetGlobalMatrix()const
{
	return global_transformation;
}

void Component_Transform::SetLocalTransform(float4x4 & trans_matrix)
{
	local_transformation = trans_matrix;
	local_transformation.Decompose(local_position, local_rotation, local_scale);

	RecalculateTransformMatrix();
}

void Component_Transform::SetGlobalTransform(float4x4 & transform_matrix)
{
	float3 position, scale;
	Quat rotation;
	transform_matrix.Decompose(position, rotation, scale);

	if (App->scene_intro->guizmo_operation == ImGuizmo::OPERATION::SCALE)
	{
		local_scale = scale;
	}

	else
	{
		local_position = position;
		local_rotation = rotation;

		euler_rotation = local_rotation.ToEulerXYZ();
		euler_rotation.x = RadToDeg(euler_rotation.x);
		euler_rotation.y = RadToDeg(euler_rotation.y);
		euler_rotation.z = RadToDeg(euler_rotation.z);
	}

	RecalculateTransformMatrix();
}

float4x4 Component_Transform::GetParentGlobalMatrix()const
{
	std::vector<GameObject*>::iterator it = gameObject_Item->game_object_childs.begin();

	float4x4 temp = (*it)->transform->GetGlobalMatrix();
	
	gameObject_Item->bbox.aabb_local.Enclose((*it)->bbox.aabb_local);
	gameObject_Item->bbox.obb = gameObject_Item->bbox.aabb_local;
	gameObject_Item->bbox.obb.Transform((*it)->transform->GetGlobalMatrix());
	it++;

	while (it < gameObject_Item->game_object_childs.end())
	{
		gameObject_Item->bbox.aabb_local.Enclose((*it)->bbox.aabb_local);

		gameObject_Item->bbox.obb = gameObject_Item->bbox.aabb_local;
		gameObject_Item->bbox.obb.Transform((*it)->transform->GetGlobalMatrix());
	//	temp = temp * (*it)->transform->GetGlobalMatrix();
		
		++it;

	}

	return temp;
}

void Component_Transform::DisplayInspector()
{

	ImGui::Text("Position  ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(50);
	ImGui::PushID(1);
	if (ImGui::DragFloat("X", &local_position.x, 0.5F)) 
	{
		RecalculateTransformMatrix();
		gameObject_Item->TransformBBox();
		if (gameObject_Item->parent->type == OBJECT_TYPE::SUBPARENT)
			gameObject_Item->parent->TransformParentBBox();

		else if (gameObject_Item->type == OBJECT_TYPE::SUBPARENT)
			gameObject_Item->TransformParentBBox();
	}
	ImGui::PopID();
	ImGui::SameLine();
	ImGui::SetNextItemWidth(50);
	ImGui::PushID(2);
	if (ImGui::DragFloat("Y", &local_position.y, 0.5F)) 
	{
		RecalculateTransformMatrix();
		gameObject_Item->TransformBBox();
		if (gameObject_Item->parent->type == OBJECT_TYPE::SUBPARENT)
			gameObject_Item->parent->TransformParentBBox();

		else if (gameObject_Item->type == OBJECT_TYPE::SUBPARENT)
			gameObject_Item->TransformParentBBox();

	}
	ImGui::PopID();
	ImGui::SameLine();
	ImGui::SetNextItemWidth(50);
	ImGui::PushID(3);
	if (ImGui::DragFloat("Z", &local_position.z, 0.5F)) 
	{
		RecalculateTransformMatrix();
		gameObject_Item->TransformBBox();
		if (gameObject_Item->parent->type == OBJECT_TYPE::SUBPARENT)
			gameObject_Item->parent->TransformParentBBox();

		else if (gameObject_Item->type == OBJECT_TYPE::SUBPARENT)
			gameObject_Item->TransformParentBBox();

	}
	ImGui::PopID();
	ImGui::Spacing();


	ImGui::Text("Rotation  ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(50);
	ImGui::PushID(4);
	if (ImGui::DragFloat("X", &euler_rotation.x, 0.5F)) 
	{
		float3 aux_rot = euler_rotation;
		aux_rot.x = DegToRad(euler_rotation.x);
		aux_rot.y = DegToRad(euler_rotation.y);
		aux_rot.z = DegToRad(euler_rotation.z);

		local_rotation = Quat::FromEulerXYZ(aux_rot.x, aux_rot.y, aux_rot.z);
		RecalculateTransformMatrix();
		gameObject_Item->TransformBBox();
		if (gameObject_Item->parent->type == OBJECT_TYPE::SUBPARENT)
			gameObject_Item->parent->TransformParentBBox();

		else if (gameObject_Item->type == OBJECT_TYPE::SUBPARENT)
			gameObject_Item->TransformParentBBox();

	}
	ImGui::PopID();

	ImGui::SameLine();
	ImGui::SetNextItemWidth(50);
	ImGui::PushID(5);
	if (ImGui::DragFloat("Y", &euler_rotation.y, 0.5F))
	{
		float3 aux_rot = euler_rotation;
		aux_rot.x = DegToRad(euler_rotation.x);
		aux_rot.y = DegToRad(euler_rotation.y);
		aux_rot.z = DegToRad(euler_rotation.z);

		local_rotation = Quat::FromEulerXYZ(aux_rot.x, aux_rot.y, aux_rot.z);
		RecalculateTransformMatrix();
		gameObject_Item->TransformBBox();
		if (gameObject_Item->parent->type == OBJECT_TYPE::SUBPARENT)
			gameObject_Item->parent->TransformParentBBox();

		else if (gameObject_Item->type == OBJECT_TYPE::SUBPARENT)
			gameObject_Item->TransformParentBBox();

	}
	ImGui::PopID();

	ImGui::SameLine();
	ImGui::SetNextItemWidth(50);
	ImGui::PushID(6);
	if (ImGui::DragFloat("Z", &euler_rotation.z, 0.5F))
	{
		float3 aux_rot = euler_rotation;
		aux_rot.x = DegToRad(euler_rotation.x);
		aux_rot.y = DegToRad(euler_rotation.y);
		aux_rot.z = DegToRad(euler_rotation.z);

		local_rotation = Quat::FromEulerXYZ(aux_rot.x, aux_rot.y, aux_rot.z);
		RecalculateTransformMatrix();
		gameObject_Item->TransformBBox();
		if (gameObject_Item->parent->type == OBJECT_TYPE::SUBPARENT)
			gameObject_Item->parent->TransformParentBBox();

		else if (gameObject_Item->type == OBJECT_TYPE::SUBPARENT)
			gameObject_Item->TransformParentBBox();

	}
	ImGui::PopID();

	ImGui::Spacing();

	ImGui::Text("Scale     ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(50);
	ImGui::PushID(7);
	if (ImGui::DragFloat("X", &local_scale.x, 0.5F))
	{
		//check if scale is negative
		RecalculateTransformMatrix();
		gameObject_Item->TransformBBox();
		if (gameObject_Item->parent->type == OBJECT_TYPE::SUBPARENT)
			gameObject_Item->parent->TransformParentBBox();

		else if (gameObject_Item->type == OBJECT_TYPE::SUBPARENT)
			gameObject_Item->TransformParentBBox();
	}
	ImGui::PopID();

	ImGui::SameLine();
	ImGui::SetNextItemWidth(50);
	ImGui::PushID(8);
	if (ImGui::DragFloat("Y", &local_scale.y, 0.5F))
	{
		//check if scale is negative
		RecalculateTransformMatrix();
		gameObject_Item->TransformBBox();
		if (gameObject_Item->parent->type == OBJECT_TYPE::SUBPARENT)
			gameObject_Item->parent->TransformParentBBox();

		else if (gameObject_Item->type == OBJECT_TYPE::SUBPARENT)
			gameObject_Item->TransformParentBBox();
	}
	ImGui::PopID();

	ImGui::SameLine();
	ImGui::SetNextItemWidth(50);
	ImGui::PushID(9);
	if (ImGui::DragFloat("Z", &local_scale.z, 0.5F))
	{
		//check if scale is negative
		RecalculateTransformMatrix();
		gameObject_Item->TransformBBox();
		if (gameObject_Item->parent->type == OBJECT_TYPE::SUBPARENT)
			gameObject_Item->parent->TransformParentBBox();

		else if (gameObject_Item->type == OBJECT_TYPE::SUBPARENT)
			gameObject_Item->TransformParentBBox();
	}
	ImGui::PopID();

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();

	ImGui::Checkbox("Bounding Box", &boundingBoxActive);

	ImGui::Spacing();

	ImGui::ColorEdit4("AABB Color", (float*)& gameObject_Item->color_aabb);
	ImGui::ColorEdit4("OBB Color", (float*)& gameObject_Item->color_obb);

	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Checkbox("Activate Gizmo", &App->ui->activate_gizmo);
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

}

void Component_Transform::SaveScene(R_JSON_Value* val)const
{
	R_JSON_Value* transform = val->NewValue(rapidjson::kObjectType);
	Quat temp;

	if (gameObject_Item->name == "Plane001") {

		gameObject_Item->transform->SetLocalRotation(0.0f, 0.0f, 0.0f, 0.0f);
		gameObject_Item->transform->SetLocalScale(0.0f, 0.0f, 0.0f);
		temp = GetLocalRotation();
	}
	else temp =  GetGlobalRotation();
	transform->SetString("Component", "Transform");

	transform->Set3DVec("Position", GetGlobalPosition());


	
	transform->SetQuat("Rotation", temp);

	transform->Set3DVec("Scale", GetGlobalScale());

	val->AddValue("Transformation", *transform);
}

bool Component_Transform::ItIntersect(LineSegment ray)
{
	AABB inter_box = gameObject_Item->bbox.aabb_global;
	if (inter_box.IsFinite())
		return ray.Intersects(inter_box);

	else
		return false;
	
}
