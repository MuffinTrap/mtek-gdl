#pragma once

#include <string>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-types.h>
using namespace gdl;

/* Contains vertex data that is needed for
 * rendering the mesh
 * positions, normals, uvs, indices
 */
struct Mesh
{
	GLushort* indices;
	GLsizei indexCount;
	u32 vertexCount;
	GLfloat* positions;
	GLfloat* normals;
	GLfloat* uvs;
	const char* name;
};

extern "C"
{
	/**
	 * @brief Reserves space for a mesh and sets mesh variables.
	 * @details Use this function to reserve memory for a mesh.
	 * @param mesh The mesh to initialize.
	 * @param vertexCount How many vertices the mesh will have.
	 * @param indexCount How many indices the mesh will have.
	 * @param createNormals Will the vertices have normals or not.
	 * @param createUVs Will the vertices have texture coordinates or not.
	 * @return Amount of bytes allocated.
	 */
	sizetype Mesh_Init(Mesh* mesh, sizetype vertexCount, sizetype indexCount, bool createNormals, bool createUVs);

	vec3 Mesh_GetPosition(Mesh* mesh, GLushort index);
	vec3 Mesh_GetNormal(Mesh* mesh, GLushort index);
	vec3 Mesh_GetPositionFromArray(Mesh* mesh, sizetype index);
	vec3 Mesh_GetNormalFromArray(Mesh* mesh, sizetype index);

	void Mesh_SetDrawingIndex(Mesh* mesh,sizetype index, GLushort drawIndex);
	void Mesh_SetPositionToArray(Mesh* mesh,sizetype index, const vec3& position);
	void Mesh_SetNormalToArray(Mesh* mesh,sizetype index, const vec3& normal);
	void Mesh_SetUVToArray(Mesh* mesh,sizetype index, const vec2& uv);
	bool Mesh_GetTriangleIndices(Mesh* mesh,GLsizei triangleIndex, GLushort& outA, GLushort& outB, GLushort& outC);

	void Mesh_DebugPrint(Mesh* mesh);


	void Mesh_SetupVertexArrays(Mesh* mesh);
	void Mesh_DrawElements(Mesh* mesh);
	void Mesh_DrawPoints(Mesh* mesh);
	void Mesh_DrawLines(Mesh* mesh);
	void Mesh_DrawNormals(Mesh* mesh);
	void Mesh_CalculateMatcapUVs(Mesh* mesh,const mat4x4& modelViewMatrix, const mat4x4& normalMatrix);

	void Mesh_SetVertexV3(vec3 vertex, u32 index);
	void Mesh_SetVertexXYZ(float x, float y, float z, u32 index);


	Mesh* Mesh_CreateIcosahedron(bool normals, bool uvs);
	Mesh* Mesh_CreateQuad(bool normals, bool uvs);

	// TODO
	void Mesh_DrawElementsPartially(Mesh* mesh, float start, float amount);

	Mesh* Mesh_CreateStar(float centerThickness, float pointRadius, float sharpness, int pointAmount);
	Mesh* Mesh_CreateStarBorder(float thickness, float pointRadius, float sharpness, int pointAmount);
	Mesh* Mesh_CreateRibbonPolygonCross(Mesh* bezierCurvePoints, int crossSectionPoints, float crossSectionRadius, int segmentsPerBezier);
	Mesh* Mesh_CreateRibbonMeshCross(Mesh* bezierCurvePoints, Mesh* crossSectionPoints, float crossSectionScale, int segmentsPerBezier);

	Mesh* Mesh_CreateCloud(float radius, int segments, float randomness);


}
