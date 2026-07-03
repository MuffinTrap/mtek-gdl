#pragma once

/**
 * @file mgdl-model.h
 * @brief Model struct for texture 3D objects
 */

struct Mesh;
struct Material;

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * @brief Model combines mesh and material
	 */
	struct Model
	{
		Mesh* m_mesh;
		Material* m_material;
	};
	typedef struct Model Model;

	/**
	 * @brief Create a new Model with no data.
	 * @returns Pointer to new model.
	 */
	Model* Model_Create();
	/**
	 * @brief Frees the memory used by model
	 * @note Does not free the texture or material
	 * @param model The model
	 * @returns nullptr
	 */
	Model* Model_Destroy(Model* model);

	/**
	 * @brief Draws a model
	 * @param model The model to draw
	 */
	void Model_Draw(Model* model);

#ifdef __cplusplus
}
#endif
