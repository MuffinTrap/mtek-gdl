#pragma once

/**
 * @file mgdl-fbx.h
 * @brief Class for loading FBX files.
 */

#include <string>
#include <mgdl/ufbx/ufbx.h>
#include <mgdl/mgdl-light.h>
#include <mgdl/mgdl-mesh.h>
#include <mgdl/mgdl-scene.h>

namespace gdl
{
	/**
	 * @brief Represents a FBX file.
	 */
	class FBXFile
	{
	public:

		/**
		 * @brief Loads a FBX file and returns the contents as a Scene.
		 *
		 * @param fbxFile Filename of the file.
		 * @return Loaded scene.
		 */
		static gdl::Scene* LoadFile(std::string fbxFile);

	private:
		static gdl::Mesh* AllocateMesh(ufbx_mesh* fbxMesh);
		static gdl::Mesh* LoadMesh(ufbx_mesh* fbxMesh);
		static gdl::Light* LoadLight(ufbx_light* fbxLight);
		static bool LoadNode(Scene* gdlScene, Node* parentNode, ufbx_node* node, short int depth);
	};
}

