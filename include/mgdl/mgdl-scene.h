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
		void DebugDraw(gdl::Font* font, short x, short y, u32 drawFlags);

		void SetActiveParentNode(gdl::Node* node);
		void PushChildNode(gdl::Node* node);

		void AddMaterial(gdl::Material* material);
		void AddMesh(gdl::Mesh* mesh);
		void AddLight(gdl::Light* light);

		void SetMaterialTexture(const std::string& materialName, gdl::Image* texture);

		gdl::Node* GetRootNode();
		gdl::Node* GetNode(const std::string& name);
		gdl::Node* GetNodeByIndex(short index);
		gdl::Mesh* GetMesh(const std::string& meshName);
		gdl::Mesh* GetMeshByUniqueId(uint32_t uniqueId);
		gdl::Light* GetLight(const std::string& lightName);
		gdl::Material* GetMaterial(const std::string& materialName);

		gdl::vec3 GetWorldPosition(gdl::Node* node);

		std::vector<gdl::Light*> lights;

	private:
		void DebugDrawNode ( Node* node, Font* font, short int x, short int& dy, short int depth, short int& index, u32 drawFlags );

		gdl::Node* FindNode(gdl::Node* node, const std::string &nodeName);
		gdl::Node* FindNodeByIndex(gdl::Node* parent, short targetIndex, short& index);
		gdl::Material* FindMaterial(gdl::Node* node, const std::string&  materialName);

		bool CalculateWorldPosition(gdl::Node* parent, gdl::Node* target, glm::mat4& matrix, gdl::vec3& posOut);

		gdl::Node* rootNode = nullptr;
		gdl::Node* parent = nullptr;
		// Multiple nodes can refer to same material
		std::vector<gdl::Material*> materials;
		// Multiple nodes can refer to same mesh
		std::vector<gdl::Mesh*> meshes;
	};
}
