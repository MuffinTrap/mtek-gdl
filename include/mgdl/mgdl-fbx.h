#pragma once

/* Class for loading and converting an FBX file
 * that contains one or more meshes and/or a scene
 */
#include <string>
#include <ufbx/ufbx.h>

namespace gdl
{
	class Mesh;
	class Scene;
	class FBXFile
	{
	public:
		gdl::Scene* LoadFile(std::string fbxFile);
		void DeleteData();

		gdl::Mesh* AllocateMesh(ufbx_mesh* fbxMesh);
		gdl::Mesh* LoadMesh(ufbx_mesh* fbxMesh);

	private:
		bool LoadNode(gdl::Scene* scene, ufbx_node* node, short depth);
		ufbx_scene* scene = nullptr;
	};
}

