#pragma once
/* This file contains types that represent a 3D scene
 *
 *
 *
 */

#include <mgdl/mgdl-fbx.h>
#include <vector>
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-image.h>
namespace gdl
{
	/* Material used by a node. Contains a
	 * pointer to Image and material properties
	 * used in rendering
	 */
	class Material
	{
	public:
		std::string name;
		gdl::Image* texture;
		void Apply();
	};

	/* Contains vertex data that is needed for
	 * rendering the mesh
	 * positions, normals, uvs, indices
	 */
	class Mesh
	{
	public:
		GLushort* indices;
		GLsizei indexCount;
		u32 vertexCount;
		GLfloat* positions;
		GLfloat* normals;
		GLfloat* uvs;

		Mesh();
		void SetupVertexArrays();
		void DrawElements();
		void DrawPoints();
		void DrawLines();

	};

	class Transform
	{
	public:
		gdl::vec3 position;
		gdl::vec3 rotationDegrees;
		float scale = 1.0f;
	};

	/* Representes a light in a 3D scene.
	 * Contains color and light properties
	 */
	class Light
	{

	};

	class Node
	{
	public:
		std::string name;
		gdl::Mesh* mesh = nullptr;
		gdl::Material* material = nullptr;
		Transform transform;
		std::vector<Node*> children;
	};


	/* Contains one or more Meshes and materials.
	 * The meshes have a hierarchy and position and rotation
	 * offsets
	 */


	/* Contains one or more lights and Models
	 */
	class Scene
	{
	public:
		bool LoadFromFBX(gdl::FBXFile* fbxFile);
		void SetActiveParentNode(gdl::Node* node);
		void PushChildNode(gdl::Node* node);
		void Draw();

		void AddMaterial(gdl::Material* material);

		void SetMaterialTexture(const std::string& materialName, gdl::Image* texture);
		gdl::Material* GetMaterial(const std::string& materialName);
		gdl::Node* GetRoot();
		gdl::Node* GetNode(const std::string& name);

	private:
		void DrawNode(gdl::Node* node);

		gdl::Node* FindNode(gdl::Node* node, const std::string &nodeName);
		gdl::Material* FindMaterial(gdl::Node* node, const std::string&  materialName);
		gdl::Node* rootNode;
		gdl::Node* parent;
		// Multiple nodes can refer to same material
		std::vector<gdl::Material*> materials;
	};
}
