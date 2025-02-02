#pragma once

/* Class for loading and converting an FBX file
 * that contains one or more meshes and/or a scene
 */
#include <string>
#include <mgdl/ufbx/ufbx.h>
#include <mgdl/mgdl-light.h>
#include <mgdl/mgdl-mesh.h>
#include <mgdl/mgdl-scene.h>

namespace gdl
{

	class FBXFile
	{
	public:
		gdl::Scene* LoadFile(std::string fbxFile, bool debugPrint = false);
		void DeleteData();


	private:
		gdl::Mesh* AllocateMesh(ufbx_mesh* fbxMesh);
		gdl::Mesh* LoadMesh(ufbx_mesh* fbxMesh);
		gdl::Light* LoadLight(ufbx_light* fbxLight);
		bool LoadNode(Scene* gdlScene, Node* parentNode, ufbx_node* node, short int depth);
		ufbx_scene* scene = nullptr;
		bool debugPrint;
	};
}

