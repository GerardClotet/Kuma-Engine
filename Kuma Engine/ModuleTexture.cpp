#include "Globals.h"
#include "ModuleTexture.h"





ModuleTexture::ModuleTexture(Application* app, bool start_enabled) :Module(app, start_enabled)
{
	 
}

ModuleTexture::~ModuleTexture()
{

}
bool ModuleTexture::CleanUp()
{

	int i = 0;
	LOG("textures size %i", textures_vec.size());
	while (i < textures_vec.size())
	{
		delete textures_vec[i];
		++i;

	}
	textures_vec.clear();

	LOG("texture module cleaned");


	return true;
}
void ModuleTexture::RemoveTexture(TexData* texture)
{

	std::vector<TexData*>::iterator iter = textures_vec.begin();
	while (iter != textures_vec.end())
	{
		if ((*iter) == texture)
		{
			textures_vec.erase(iter);
			delete texture;
			return;
		}
	}
}
bool ModuleTexture::Init()
{
	bool ret = true;
	//ILuint init;
	ILuint devIl_init;


	ilInit();
	devIl_init = ilGetError();
	if (devIl_init != IL_NO_ERROR)
		ret = false;
	
	iluInit();
	devIl_init = ilGetError();
	if (devIl_init != IL_NO_ERROR)
		ret = false;


	ilutRenderer(ILUT_OPENGL);

	devIl_init = ilGetError();
	if (devIl_init != IL_NO_ERROR)
		ret = false;


	return true;
}

TexData* ModuleTexture::LoadTexture(const char* path)
{

	ILuint id;
		

	ilGenImages(1, &id);

	ilBindImage(id);
	if ((bool)ilLoadImage(path))
	{
		iluFlipImage();
		
		tex_data = new TexData;


		if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glGenTextures(1, &tex_data->id);
			glBindTexture(GL_TEXTURE_2D, tex_data->id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
				0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());



			tex_data->img_data = ilGetData();
			tex_data->width = ilGetInteger(IL_IMAGE_WIDTH);
			tex_data->height = ilGetInteger(IL_IMAGE_HEIGHT);
			tex_data->offsetX = ilGetInteger(IL_IMAGE_OFFX);
			tex_data->offsetY = ilGetInteger(IL_IMAGE_OFFY);

			textures_vec.push_back(tex_data);
		}
		else LOG("Failure converting image, error: %s", iluErrorString(ilGetError()));



	}

	else LOG("Error Loading texture %s", iluErrorString(ilGetError()));


	return tex_data;
}



