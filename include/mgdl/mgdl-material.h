#pragma once

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
	GLfloat shininess;
	vec3 emissiveColor;
	Image* texture;
	MaterialType type;
};
typedef struct Material Material;

#ifdef __cplusplus
extern "C"
{
#endif
	/**
	 * @brief Creates a material from an image.
	 * @param name Name for the material.
	 * @param texture Image to use for the material.
	 * @param type What kind of material this is.
	 * @returns Created material.
	 */
	Material* Material_Load(const char* name, Image* texture, MaterialType type);

	/**
	 * @brief Applies the material properties for the next mesh drawing operation
	 * @param material The material properties to use.
	 */
	void Material_Apply(Material* material);

#ifdef __cplusplus
}
#endif
