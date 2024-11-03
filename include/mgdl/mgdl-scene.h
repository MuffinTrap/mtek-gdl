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

namespace gdl
{
	// Hiearchy of Nodes
	class Scene
	{
	public:
		void Draw();

		void SetActiveParentNode(gdl::Node* node);
		void PushChildNode(gdl::Node* node);

		void AddMaterial(gdl::Material* material);
		void AddMesh(gdl::Mesh* mesh);
		void AddLight(gdl::Light* light);

		void SetMaterialTexture(const std::string& materialName, gdl::Image* texture);

		gdl::Node* GetRootNode();
		gdl::Node* GetNode(const std::string& name);
		gdl::Mesh* GetMesh(const std::string& meshName);
		gdl::Light* GetLight(const std::string& lightName);
		gdl::Material* GetMaterial(const std::string& materialName);

		std::vector<gdl::Light*> lights;

	private:
		void DrawNode(gdl::Node* node);

		gdl::Node* FindNode(gdl::Node* node, const std::string &nodeName);
		gdl::Material* FindMaterial(gdl::Node* node, const std::string&  materialName);

		gdl::Node* rootNode;
		gdl::Node* parent;
		// Multiple nodes can refer to same material
		std::vector<gdl::Material*> materials;
		// Multiple nodes can refer to same mesh
		std::vector<gdl::Mesh*> meshes;
	};
}
