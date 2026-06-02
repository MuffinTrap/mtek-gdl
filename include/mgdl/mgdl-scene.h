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

enum Scene_DebugFlag : u32
{
	Index = 1,
	UniqueId = 2,
	Position = 4,
	Rotation = 8
};

struct Scene
{
	ufbx_scene* ufbx;
	Node* rootNode;
	// Multiple nodes can refer to same material
	struct DynamicArray* materials;
	// Multiple nodes can refer to same mesh
	struct DynamicArray* meshes;
	struct DynamicArray* lights;

};
typedef struct Scene Scene;

DYNAMIC_ARRAY(Mesh)
DYNAMIC_ARRAY(Light)
DYNAMIC_ARRAY(Material)

#ifdef __cplusplus
extern "C"
{
#endif

	Scene* Scene_CreateEmpty(void);
	void Scene_Init(Scene* scene);

	void Scene_Draw(Scene* scene);
	void Scene_DrawNode(Node* node);
	void Scene_DebugDraw(Scene* scene, Menu* menu, short x, short y, u32 debugFlags);
	void Scene_AddChildNode(Scene* scene, Node* parent, Node* child);
	void Scene_AddMaterial(Scene* scene, Material* material);
	void Scene_SetMaterialTexture(Scene* scene, const char* materialName, Texture* texture);
	void Scene_SetAllMaterialTextures(Scene* scene, Texture* texture);

	Node* Scene_GetRootNode(Scene* scene);
	Material* Scene_GetMaterial(Scene* scene, const char* materialName);

	Vector3 Scene_GetNodePosition(Scene* scene, Node* node);
	bool Scene_GetNodeModelMatrix(Scene* scene, Node* node, Matrix modelOut);

	void Scene_DebugDrawNode_(Node* node, Menu* menu, short depth, short* index, u32 drawFlags );

	Material* Scene_FindNodeMaterial(Scene* scene, Node* node, const char*  materialName);

	bool Scene_CalculateNodePosition(Node* parent, Node* target, Matrix world, Vector3* posOut);
	bool Scene_CalculateNodeModelMatrix(Node* parent, Node* target, Matrix model);


#ifdef __cplusplus
}
#endif
