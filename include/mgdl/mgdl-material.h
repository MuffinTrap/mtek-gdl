#pragma once

#include <string>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-image.h>


/**
 * @brief Material used by a node.
 * @details Contains a pointer to Image and material properties used in rendering.
 */
struct Material
{
	char* name;
	GLfloat shininess = 1.0f;
	vec3 emissiveColor;
	Image* texture;
	gdl::MaterialType type;
};

extern "C"
{
	/**
	 * @brief Creates a material from an image.
	 * @param name Name for the material.
	 * @param texture Image to use for the material.
	 * @param type What kind of material this is.
	 * @returns Created material.
	 */
	Material* Material_Load(const char* name, Image* texture, gdl::MaterialType type);

	/**
	 * @brief Applies the material properties for the next mesh drawing operation
	 * @param material The material properties to use.
	 */
	void Material_Apply(Material* material);

}
