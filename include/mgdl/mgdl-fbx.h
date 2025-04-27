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


/**
 * @brief Loads a FBX file and returns the contents as a Scene.
 *
 * @param fbxFile Filename of the file.
 * @return Loaded scene.
 */
Scene* FBX_LoadScene(const char* fbxFile);

gdl::Mesh* _FBX_AllocateMesh(ufbx_mesh* fbxMesh);
gdl::Mesh* _FBX_LoadMesh(ufbx_mesh* fbxMesh);
Light* _FBX_LoadLight(ufbx_light* fbxLight);
bool _FBX_LoadNode(Scene* gdlScene, Node* parentNode, ufbx_node* node, short int depth);

