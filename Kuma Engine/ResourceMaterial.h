#pragma once

#include "Resource.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"
struct aiMaterial;
struct aiString;
class ResourceMaterial : public Resource
{
public:
	enum Texture
	{
		TextureDiffuse = 0,
		TextureSpecular,
		TextureNormal,
		TextureOcclusion,
		TextureEmissive,
		TextureCount
	};

public:
	ResourceMaterial(UID id);
	virtual ~ResourceMaterial();

	void Import(const aiMaterial* material, const char* base_path);
	void ImportTexture(const char* path);
	bool CheckTextureMeta(const char* path,std::string&new_path);

	float4      diffuse_color = float4::one;
	float3      specular_color = float3::zero;
	float3      emissive_color = float3::zero;

	UID         textures[TextureCount] = { 0, 0, 0, 0, 0 }; //now just importing diffuse
	float       shininess = 0.5f;
	float       k_ambient = 0.5f;
	float       k_diffuse = 0.5f;
	float       k_specular = 0.5f;

	

public:
	UID texture_id;
	std::string tex_path;
	bool checkers = false;
};
