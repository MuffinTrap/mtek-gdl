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

		vec3 GetPositionFromArray(sizetype index);
		vec3 GetNormalFromArray(sizetype index);

		void SetDrawingIndex(sizetype index, GLushort drawIndex);
		void SetPositionToArray(sizetype index, const vec3& position);
		void SetNormalToArray(sizetype index, const vec3& normal);
		void SetUVToArray(sizetype index, const vec2& uv);

		bool GetTriangleIndices(GLsizei triangleIndex, GLushort& outA, GLushort& outB, GLushort& outC);

		void DebugPrint();

		Mesh();
		// Returns the amount of bytes reserved
		sizetype Allocate(sizetype vertexCount, sizetype indexCount, bool createNormals, bool createUVs);
		void SetupVertexArrays();
		void DrawElements();
		void DrawPoints();
		void DrawLines();
		void DrawNormals();
		void CalculateMatcapUVs(const mat4x4& modelViewMatrix, const mat4x4& normalMatrix);

		static Mesh* CreateIcosahedron(bool normals, bool uvs);
		static Mesh* CreateQuad(bool normals, bool uvs);
	};

}
