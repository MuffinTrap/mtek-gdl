#pragma once

/**
 * @file mgdl-fbx.h
 * @ingroup asset
 * @brief Class for loading FBX files.
 */

#include <mgdl/ufbx/ufbx.h>
#include <mgdl/mgdl-light.h>

struct Mesh;
struct Model;
struct Scene;
struct Node;
struct Material;

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
Model* FBX_LoadFirstModel(const char* fbxFile);

Mesh* m_FBX_AllocateMesh(ufbx_mesh* fbxMesh);
Mesh* m_FBX_LoadMesh(ufbx_mesh* fbxMesh);
Light* m_FBX_LoadLight(ufbx_light* fbxLight);
bool m_FBX_LoadNode(Scene* gdlScene, Node* parentNode, ufbx_node* node, s16 depth);

Mesh* FBX_LoadMeshTrianglesOnly(ufbx_mesh* mesh);

Material* FBX_LoadFirstMaterial(ufbx_scene* scene, const char* searchfolder);
Material* FBX_LoadNodeMaterial(ufbx_node* node, int materialIndex, const char* searchfolder);

ufbx_scene* FBX_LoadScene(const char* fbxFile);
ufbx_mesh* FBX_GetFirstMesh(ufbx_scene* scene);
Model* FBX_LoadFirstModel(const char* fbxFile);

#ifdef __cplusplus
}
#endif
