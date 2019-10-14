#include "Component_Material.h"

Component_Material::Component_Material()
{
	comp_type = GO_COMPONENT::MATERIAL;
}

Component_Material::~Component_Material()
{
}

bool Component_Material::Update()
{
	return true;
}

bool Component_Material::Enable()
{
	return true;
}

bool Component_Material::Disable()
{
	return true;
}
