#include "Module.h"


#include "DevIL/include/IL/ilu.h"
#include "DevIL/include/IL/ilut.h"
#include "DevIL/include/IL/il.h"

#pragma comment(lib,"Devil/libx86/DevIL.lib")
#pragma comment(lib,"Devil/libx86/ILU.lib")
#pragma comment(lib,"Devil/libx86/ILUT.lib")

#include <vector>
struct TexData
{	
	int width=0;
	int height=0;
	int offsetX=0;
	int offsetY=0;
	GLuint id=0;
	GLubyte img=0;
	unsigned char* img_data=nullptr;
};

class ModuleTexture : public Module 
{

public:
	ModuleTexture(Application* app, bool start_enabled = true);
	~ModuleTexture();


	bool Init();
	bool CleanUp();

	TexData* LoadTexture(const char* path);

	//ilLoadImage allows users to transparently load several different image formats uniformly.
	TexData* tex_data = nullptr;
	std::vector <TexData*> textures_vec;
};

