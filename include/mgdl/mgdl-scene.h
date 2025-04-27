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
#include <mgdl/mgdl-font.h>

#include <glm/glm.hpp>

namespace gdl
{
	// Hiearchy of Nodes
	class Scene
	{
	public:
		enum DebugFlag : u32
		{
			Index = 1,
			UniqueId = 2,
			Position = 4,
			Rotation = 8
		};
		Scene();
		void Draw();
		void DrawNode(gdl::Node* node);
		void DebugDraw(Font* font, short x, short y, u32 drawFlags);

		void AddChildNode(gdl::Node* parent, gdl::Node* child);

		void AddMaterial(gdl::Material* material);
		void SetMaterialTexture(const char* materialName, Image* texture);
		void SetAllMaterialTextures(Image* texture);

		gdl::Node* GetRootNode();
		gdl::Node* GetNode(const char* name);
		gdl::Node* GetNodeByIndex(short index);
		gdl::Material* GetMaterial(const char* materialName);

		vec3 GetWorldPosition(gdl::Node* node);
		bool GetModelMatrix(gdl::Node* node, mat4x4 modelOut);


	private:
		void DebugDrawNode ( Node* node, Font* font, short int x, short int& dy, short int depth, short int& index, u32 drawFlags );

		gdl::Node* FindNode(gdl::Node* node, const char* nodeName);
		gdl::Node* FindNodeByIndex(gdl::Node* parent, short targetIndex, short& index);
		gdl::Material* FindMaterial(gdl::Node* node, const char*  materialName);

		bool CalculateWorldPosition(gdl::Node* parent, gdl::Node* target, mat4x4 world, vec3& posOut);
		bool CalculateModelMatrix(gdl::Node* parent, gdl::Node* target, mat4x4 model);

		gdl::Node* rootNode = nullptr;
		// Multiple nodes can refer to same material
		std::vector<gdl::Material*> materials;
		// Multiple nodes can refer to same mesh
		std::vector<gdl::Mesh*> meshes;
		std::vector<gdl::Light*> lights;
	};
}
