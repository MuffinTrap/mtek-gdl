#pragma once

/**
 * @file mgdl-fbx.h
 * @ingroup asset
 * @brief Class for loading FBX files.
 */

#include <mgdl/ufbx/ufbx.h>
#include <mgdl/mgdl-light.h>
#include <mgdl/mgdl-mesh.h>
#include <mgdl/mgdl-scene.h>

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @brief Loads a FBX file and returns the contents as a Scene.
 *
 * @param fbxFile Filename of the file.
 * @return Loaded scene.
 */
Scene* FBX_Load(const char* fbxFile);

Mesh* m_FBX_AllocateMesh(ufbx_mesh* fbxMesh);
Mesh* m_FBX_LoadMesh(ufbx_mesh* fbxMesh);
Light* m_FBX_LoadLight(ufbx_light* fbxLight);
bool m_FBX_LoadNode(Scene* gdlScene, Node* parentNode, ufbx_node* node, short int depth);

Mesh* FBX_LoadMeshTrianglesOnly(ufbx_mesh* mesh);


ufbx_scene* FBX_LoadScene(const char* fbxFile);
ufbx_mesh* FBX_GetFirstMesh(ufbx_scene* scene);

#ifdef __cplusplus
}
#endif
