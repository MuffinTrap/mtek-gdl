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
	/* Material used by a mesh. Contains a
	 * pointer to Image and material properties
	 * used in rendering
	 */
	class Material
	{

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

	/* Contains one or more Meshes and materials.
	 * The meshes have a hierarchy and position and rotation
	 * offsets
	 */
	class Model
	{
	public:
		void AddMesh(Mesh* meshIn);
		void AddTexture(gdl::Image* imageIn);
		void AddTexturedMesh(Mesh* meshIn, gdl::Image* imageIn);

		void Draw(int texture = 0);
		std::vector<Mesh*> meshes;
		std::vector<gdl::Image*> textures;
	};

	/* Representes a light in a 3D scene.
	 * Contains color and light properties
	 */
	class Light
	{

	};

	/* Contains one or more lights and Models
	 */
	class Scene
	{
	public:
		bool LoadFromFBX(gdl::FBXFile* fbxFile);
		std::vector<Model*> models;
	};
}
