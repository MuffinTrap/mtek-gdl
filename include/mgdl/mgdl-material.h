#pragma once

#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-texture.h>

/**
 * @file mgdl-material.h
 * @ingroup scene
 * @brief Material struct and functions
 */

/**
 * @brief Material used by a node.
 * @details Contains a pointer to Texture and material properties used in rendering.
 */
struct Material
{
	char* name;
	GLfloat shininess;
	GLfloat emissiveColor[4];
	GLfloat diffuseColor[4];
	Texture* texture;
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
	 * @param texture Texture to use for the material.
	 * @param type What kind of material this is.
	 * @returns Created material.
	 */
	Material* Material_Load(const char* name, Texture* texture, MaterialType type);

	Material* Material_CreateColor(Color4f color, GLfloat shininess, GLfloat emissionPower);

	/**
	 * @brief Applies the material properties for the next mesh drawing operation
	 * @param material The material properties to use.
	 */
	void Material_Apply(Material* material);
	void Material_Reset(void);

#ifdef __cplusplus
}
#endif
