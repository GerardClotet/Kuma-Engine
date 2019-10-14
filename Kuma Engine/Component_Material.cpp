#include "Component_Material.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "Application.h"
#include "ModuleTexture.h"



Component_Material::Component_Material()
{
	comp_type = GO_COMPONENT::MATERIAL;
}

Component_Material::~Component_Material()
{
}

bool Component_Material::Update()
{
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);

	if (texture != nullptr)
		glBindTexture(GL_TEXTURE_2D, texture->id);

	glDisableClientState(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
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
