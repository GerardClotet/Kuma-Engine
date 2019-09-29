#include "Globals.h"

#include "Application.h"
#include "ModuleHardware.h"
#include "SDL/include/SDL_cpuinfo.h"

ModuleHardware::ModuleHardware(Application* app, bool start_enabled) : Module(app, start_enabled)
{

	SDL_version version;
	SDL_GetVersion(&version);

	sprintf_s(sdl_version, 20, "%i.%i.%i", version.major, version.minor, version.patch);
	cpu_cores = SDL_GetCPUCount();
	l1_cpu_cache_bytes = SDL_GetCPUCacheLineSize();
	system_RAM_gb = (float)SDL_GetSystemRAM() / TWO_Raised_to_10_in_bytes;
	has3D_now = SDL_Has3DNow();
	avx = SDL_HasAVX();
	avx2 = SDL_HasAVX2();
	altivec = SDL_HasAltiVec();
	mmx = SDL_HasMMX();
	rdtsc = SDL_HasRDTSC();
	sse = SDL_HasSSE();
	sse2 = SDL_HasSSE2();
	sse3 = SDL_HasSSE3();
	sse41 = SDL_HasSSE41();
	sse42 = SDL_HasSSE42();

	//GPU spec initialized in renderer3D
	
	

	App->saveLog(sdl_version);
	
}

ModuleHardware::~ModuleHardware()
{

}



update_status ModuleHardware::Update(float dt)
{

	return UPDATE_CONTINUE;
}


