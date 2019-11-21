#include "Module.h"


#include "DevIL/include/IL/ilu.h"
#include "DevIL/include/IL/ilut.h"
#include "DevIL/include/IL/il.h"

#pragma comment(lib,"Devil/libx86/DevIL.lib")
#pragma comment(lib,"Devil/libx86/ILU.lib")
#pragma comment(lib,"Devil/libx86/ILUT.lib")

#include <vector>

#define CHECKERS_WIDTH 100	
#define CHECKERS_HEIGHT 100

class ResourceTexture;
struct TexData
{	
	std::string name;
	std::string naem_extension;
	int width=0;
	int height=0;
	int offsetX=0;
	int offsetY=0;
	GLuint id=0;
	GLubyte img=0;
	unsigned char* img_data=nullptr;
	byte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
};

class ModuleTexture : public Module 
{

public:
	ModuleTexture(Application* app, bool start_enabled = true);
	~ModuleTexture();

	void CheckersTexture();
	bool Init();
	bool Start();
	bool CleanUp();
	void RemoveTexture(TexData* texture);
	TexData* LoadTexture(const char* path);
	TexData* GetDefaultTex();
	//ilLoadImage allows users to transparently load several different image formats uniformly.
	TexData* tex_data = nullptr;
	std::vector <TexData*> textures_vec;
	TexData* CheckAlreadyLoaded(const char* path);

	//SAVE ONLY dds format
	void SaveTextureTo(const char* base_path, const char* save_to);



	//RESOURCES

	bool LoadResourceTexture(ResourceTexture* resource);
public:
	uint ImageName;
	

};

