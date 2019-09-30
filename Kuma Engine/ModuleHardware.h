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
	
	uint gpu_vendor;
	uint gpu_device;
	std::wstring brand;
	unsigned __int64 vid_mem_budget;
	unsigned __int64 vid_mem_usage;
	unsigned __int64 vid_mem_available;
	unsigned __int64 vid_mem_reserved;

	float vram_mb_budget;
	float vram_mb_usage;
	float vram_mb_available;
	float vram_mb_reserved;

	char gpu_brand[250] = "";
	char sdl_version[20]="";
};