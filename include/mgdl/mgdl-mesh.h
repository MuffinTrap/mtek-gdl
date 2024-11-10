#pragma once

#include <string>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-types.h>

namespace gdl
{
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
		std::string name;

		gdl::vec3 GetPosition(GLushort index);
		gdl::vec3 GetNormal(GLushort index);
		void SetNormal(GLsizei index, const gdl::vec3& normal);

		bool GetTriangleIndices(GLsizei triangleIndex, GLushort& outA, GLushort& outB, GLushort& outC);

		Mesh();
		void SetupVertexArrays();
		void DrawElements();
		void DrawPoints();
		void DrawLines();
		void DrawNormals();
	};
}
