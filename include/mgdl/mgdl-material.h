#pragma once

#include <string>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-image.h>


/* Material used by a node. Contains a
 * pointer to Image and material properties
 * used in rendering
 */
struct Material
{
	const char* name;
	GLfloat shininess = 1.0f;
	vec3 emissiveColor;
	Image* texture;
	gdl::MaterialType type;
};

void Material_Init(Material* material, const char* name, Image* texture, gdl::MaterialType type);

void Material_Apply(Material* material);
