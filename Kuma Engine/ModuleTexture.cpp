#include "Application.h"
#include "Globals.h"
#include "ModuleTexture.h"
#include "ModuleFileSystem.h"



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
			if ((*iter)->name == "default texture")
				return;
			textures_vec.erase(iter);
			delete texture;
			return;
		}
		iter++;
	}
}
void ModuleTexture::CheckersTexture()
{

	tex_data = new TexData;

	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			tex_data->checkImage[i][j][0] = (GLubyte)c;
			tex_data->checkImage[i][j][1] = (GLubyte)c;
			tex_data->checkImage[i][j][2] = (GLubyte)c;
			tex_data->checkImage[i][j][3] = (GLubyte)255;
		}
	}
	;
	LOG("");


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &(GLuint)tex_data->id);
	glBindTexture(GL_TEXTURE_2D, (GLuint)tex_data->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data->checkImage);
	tex_data->name = "default texture";
	textures_vec.push_back(tex_data);
	
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

bool ModuleTexture::Start()
{
	CheckersTexture();

	return true;
}

TexData* ModuleTexture::LoadTexture(const char* path)
{

	ILuint id;
	
	TexData* cmp_tex = CheckAlreadyLoaded(App->fs->GetFileName(path, true).c_str());

	if (cmp_tex != nullptr)
		return cmp_tex;
		
	ilGenImages(1, &id);

	ilBindImage(id);
	if ((bool)ilLoadImage(path)) //meta fail
	{
		iluFlipImage();
		
		cmp_tex = new TexData;


		if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		{
			
			cmp_tex->naem_extension = App->fs->GetFileName(path, true);

			
			cmp_tex->name = path;

			
			//cmp_tex->name = cmp_tex->name.substr(tex_data->name.find_last_of("/") + 1);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glGenTextures(1, &cmp_tex->id);
			glBindTexture(GL_TEXTURE_2D, cmp_tex->id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
				0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());



			cmp_tex->img_data = ilGetData();
			cmp_tex->width = ilGetInteger(IL_IMAGE_WIDTH);
			cmp_tex->height = ilGetInteger(IL_IMAGE_HEIGHT);
			cmp_tex->offsetX = ilGetInteger(IL_IMAGE_OFFX);
			cmp_tex->offsetY = ilGetInteger(IL_IMAGE_OFFY);

			textures_vec.push_back(cmp_tex);
		}
		else
		{
			LOG("Failure converting image, error: %s", iluErrorString(ilGetError())); //gives error whe laoding from meta, says the path has de dirtypart

			return nullptr;
		}



	}

	else LOG("Error Loading texture %s", iluErrorString(ilGetError()));


	return cmp_tex;
}

TexData* ModuleTexture::GetDefaultTex()
{

	std::vector<TexData*>::iterator it = textures_vec.begin();
	while (it < textures_vec.end())
	{
		if ((*it)->img_data == nullptr)
			return((*it));


		++it;
	}
	LOG("Default texture missing");
	return nullptr;
}

TexData* ModuleTexture::CheckAlreadyLoaded(const char* path) //TODO ajust path to filename.extension to avoid loading the same tex twice because its loaded from different folder
{

	std::vector<TexData*>::iterator it = textures_vec.begin();
	while (it < textures_vec.end())
	{
		if (strcmp((*it)->naem_extension.c_str(), path) == 0)
		{
			LOG("Texture %s alreadyLoaded", path);
			return (*it);
		}
		++it;
	}
	return nullptr;
}

void ModuleTexture::SaveTextureTo(const char* base_path, const char* save_to)
{
	std::string output;
	ILuint size;
	ILubyte* data;

	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
	size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
	if (size > 0) {
		data = new ILubyte[size]; // allocate data buffer
		iluFlipImage();
		if (ilSaveL(IL_DDS, data, size) > 0)
		{
			// Save to buffer with the ilSaveIL function
			std::string temp = App->fs->GetFileName(base_path);
			App->fs->SaveUnique(output, data, size, save_to, temp.c_str(), ".dds");
		}
		RELEASE_ARRAY(data);
	}
}

bool ModuleTexture::LoadResourceTexture(ResourceTexture* resource)
{
	return false;
}



