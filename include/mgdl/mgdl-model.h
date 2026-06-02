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

	struct Model
	{
		Mesh* m_mesh;
		Material* m_material;
	};
	typedef struct Model Model;

	Model* Model_Create();
	Model* Model_Destroy(Model* model);

	void Model_Draw(Model* model);

#ifdef __cplusplus
}
#endif
