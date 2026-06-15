#pragma once
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-texture.h>
#include <mgdl/mgdl-material.h>
#include <mgdl/mgdl-mesh.h>
#include <mgdl/mgdl-light.h>
#include <mgdl/mgdl-gui.h>
#include <mgdl/mgdl-vectorfunctions.h>
#include <mgdl/ufbx/ufbx.h>
#include <mgdl/mgdl-dynamic_array.h>

struct Node;

/**
 * @file mgdl-scene.h
 * @ingroup scene draw3d
 * @brief This file contains types that represent a 3D scene
 */

struct Scene
{
	ufbx_scene* ufbx;
	Node* rootNode;
	PointerArray* materials;
	PointerArray* meshes;
	PointerArray* lights;

};
typedef struct Scene Scene;

POINTER_ARRAY_DECLARE(Mesh)
POINTER_ARRAY_DECLARE(Light)
POINTER_ARRAY_DECLARE(Material)

#ifdef __cplusplus
extern "C"
{
#endif

	Scene* Scene_CreateEmpty(void);
	/**
	 * @brief Initializes scene with empty dynamic arrays
	 * @param meshCapacity How many meshes
	 * @param materialCapacity How many materials
	 * @param lightCapacity How many lights. Maximum value is 8
	 */
	void Scene_InitArrays(Scene* scene, int meshCapacity, int materialCapacity, int lightCapacity);

	void Scene_DrawFbx(Scene* scene);
	void Scene_SetUFBX(Scene* scene, ufbx_scene* ufbxScene);

	void Scene_AddMesh(Scene* scene, Mesh* mesh);
	void Scene_AddLight(Scene* scene, Light* light);
	void Scene_AddMaterial ( Scene* scene, Material* material );

	bool Scene_HasMaterial(Scene* scene, uint32_t ufbx_id);
	bool Scene_HasMesh(Scene* scene, uint32_t ufbx_id);
	void Scene_SetMaterialTexture (Scene* scene, uint32_t ufbx_id, Texture* texture );
	void Scene_SetAllMaterialTextures (Scene* scene, Texture* texture );

	void Scene_LogInfo(Scene* scene);


#ifdef __cplusplus
}
#endif
