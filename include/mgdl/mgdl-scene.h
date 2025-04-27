#pragma once
/* This file contains types that represent a 3D scene
 *
 */
#include <vector>
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-image.h>
#include <mgdl/mgdl-mesh.h>
#include <mgdl/mgdl-material.h>
#include <mgdl/mgdl-node.h>
#include <mgdl/mgdl-gui.h>
#include <glm/glm.hpp>

using namespace gdl;

	enum Scene_DebugFlag : u32
	{
		Index = 1,
		UniqueId = 2,
		Position = 4,
		Rotation = 8
	};

	struct Scene
	{
		Node* rootNode = nullptr;
		// Multiple nodes can refer to same material
		std::vector<Material*> materials;
		// Multiple nodes can refer to same mesh
		std::vector<Mesh*> meshes;
		std::vector<Light*> lights;

	};
		void Scene_Init(Scene* scene);

		void Scene_Draw(Scene* scene);
		void Scene_DrawNode(Node* node);
		void Scene_DebugDraw(Scene* scene, MenuCreator* menu, short x, short y, u32 drawFlags);
		void Scene_AddChildNode(Scene* scene, Node* parent, Node* child);
		void Scene_AddMaterial(Scene* scene, Material* material);
		void Scene_SetMaterialTexture(Scene* scene, const char* materialName, Image* texture);
		void Scene_SetAllMaterialTextures(Scene* scene, Image* texture);

		Node* Scene_GetRootNode(Scene* scene);
		Node* Scene_GetNode(Scene* scene, const char* name);
		Node* Scene_GetNodeByIndex(Scene* scene, short index);
		Material* Scene_GetMaterial(Scene* scene, const char* materialName);

		vec3 Scene_GetNodePosition(Scene* scene, Node* node);
		bool Scene_GetNodeModelMatrix(Scene* scene, Node* node, mat4x4 modelOut);

		void Scene_DebugDrawNode (Node* node, MenuCreator* menu, short int depth, short int& index, u32 drawFlags );

		Node* Scene_FindChildNode(Node* node, const char* nodeName);
		Node* Scene_FindChildNodeByIndex(Node* parent, short targetIndex, short& index);
		Material* Scene_FindNodeMaterial(Scene* scene, Node* node, const char*  materialName);

		bool Scene_CalculateNodePosition(Node* parent, Node* target, mat4x4 world, vec3& posOut);
		bool Scene_CalculateNodeModelMatrix(Node* parent, Node* target, mat4x4 model);

