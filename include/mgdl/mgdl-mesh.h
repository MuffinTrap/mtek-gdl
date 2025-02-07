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
		uint32_t uniqueId;

		vec3 GetPosition(GLushort index);
		vec3 GetNormal(GLushort index);

		vec3 GetPositionFromArray(size_t index);
		vec3 GetNormalFromArray(size_t index);

		void SetNormal(GLsizei index, const vec3& normal);

		bool GetTriangleIndices(GLsizei triangleIndex, GLushort& outA, GLushort& outB, GLushort& outC);

		Mesh();
		void SetupVertexArrays();
		void DrawElements();
		void DrawPoints();
		void DrawLines();
		void DrawNormals();
	};
}
