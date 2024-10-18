#pragma once

/* Class for loading and converting an FBX file
 * that contains one or more meshes and/or a scene
 */
#include <string>
#include <ufbx/ufbx.h>

namespace gdl
{
	class Mesh;
	class FBXFile
	{
	public:
		bool LoadFile(std::string fbxFile);
		void DeleteData();
		ufbx_mesh* GetMesh(int index);

		gdl::Mesh* LoadMesh(ufbx_mesh* fbxMesh);

	private:
		ufbx_scene* scene = nullptr;
	};
}

