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
	zstr name;
	GLfloat shininess;
	GLfloat emissiveColor[4];
	GLfloat diffuseColor[4];
	Texture* texture;
	MaterialType type;

	// ufbx connection
	uint32_t ufbx_id;
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

	Material* Material_CreateColor(color32 color, GLfloat shininess);
	void Material_SetDiffuseColor(Material* material, color32 color);

	/**
	 * @brief Applies the material properties for the next mesh drawing operation
	 * @param material The material properties to use.
	 */
	void Material_Apply(Material* material);
	/**
	 * @brief Reverts the material properties
	 * @param material The material properties to use.
	 */
	void Material_UnApply(Material* material);
	void Material_Reset(void);
	void Material_Free(Material* material);

#ifdef __cplusplus
}
#endif
