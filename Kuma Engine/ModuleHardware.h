#pragma once

#include "Module.h"
#define TWO_Raised_to_10_in_bytes 1024.0f
class ModuleHardware : public Module
{
public:
	ModuleHardware(Application* app, bool start_enabled = true);
	~ModuleHardware();

	update_status Update(float dt);

	//std::string version;
	int cpu_cores;
	int l1_cpu_cache_bytes;
	float system_RAM_gb;
	bool has3D_now;
	bool avx;
	bool avx2;
	bool altivec;
	bool mmx;
	bool rdtsc;
	bool sse;
	bool sse2;
	bool sse3;
	bool sse41;
	bool sse42;

	char sdl_version[20]="";
};