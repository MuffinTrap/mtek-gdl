#pragma once
/* This file contains types that represent a 3D scene
 *
 */
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-texture.h>
#include <mgdl/mgdl-mesh.h>
#include <mgdl/mgdl-material.h>
#include <mgdl/mgdl-node.h>
#include <mgdl/mgdl-gui.h>
#include <mgdl/mgdl-vectorfunctions.h>

enum Scene_DebugFlag : u32
{
	Index = 1,
	UniqueId = 2,
	Position = 4,
	Rotation = 8
};
struct DynamicArray;
struct Scene
{
	Node* rootNode;
	// Multiple nodes can refer to same material
	struct DynamicArray* materials;
	// Multiple nodes can refer to same mesh
	struct DynamicArray* meshes;
	struct DynamicArray* lights;

};
typedef struct Scene Scene;
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

	V3f Scene_GetNodePosition(Scene* scene, Node* node);
	bool Scene_GetNodeModelMatrix(Scene* scene, Node* node, MTX4x4 modelOut);

	void Scene_DebugDrawNode_(Node* node, Menu* menu, short depth, short* index, u32 drawFlags );

	Material* Scene_FindNodeMaterial(Scene* scene, Node* node, const char*  materialName);

	bool Scene_CalculateNodePosition(Node* parent, Node* target, MTX4x4 world, V3f* posOut);
	bool Scene_CalculateNodeModelMatrix(Node* parent, Node* target, MTX4x4 model);


#ifdef __cplusplus
}
#endif
