#pragma once

#include <ogldev/texture.h>
#include <ogldev/vec3f.h>

struct PBRMaterial
{
	float Roughness = 0.0f;
	bool IsMetal = false;
	Vector3f Color = Vector3f(0.0f, 0.0f, 0.0f);
};

class Material
{

public:
	Vector3f AmbientColor = Vector3f(0.0f, 0.0f, 0.0f);
	Vector3f DiffuseColor = Vector3f(0.0f, 0.0f, 0.0f);
	Vector3f SpecularColor = Vector3f(0.0f, 0.0f, 0.0f);

	PBRMaterial PBRmaterial;

	// TODO: need to deallocate these
	Texture* pDiffuse = NULL; // base color of the material
	Texture* pSpecularExponent = NULL;
};