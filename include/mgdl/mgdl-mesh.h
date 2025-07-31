#pragma once

#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-types.h>

/* Contains vertex data that is needed for
 * rendering the mesh
 * positions, normals, uvs, indices
 */
struct Mesh
{
	GLushort* indices;
	GLsizei indexCount;
	u32 vertexCount;
	GLushort indexCounter; /**< Used when creating a mesh */
	GLfloat* positions;
	GLfloat* normals;
	GLfloat* uvs;
	GLfloat* colors;
	const char* name;
};
typedef struct Mesh Mesh;


#ifdef __cplusplus
extern "C"
{
#endif
	/**
	 * @brief Reserves space for a mesh and sets mesh variables.
	 * @details Use this function to reserve memory for a mesh.
	 * @param mesh The mesh to initialize.
	 * @param vertexCount How many vertices the mesh will have.
	 * @param indexCount How many indices the mesh will have.
	 * @param createFlags Collection of flags specifying what vertex attributes to create
	 * @return Amount of bytes allocated.
	 */
	sizetype Mesh_Init(Mesh* mesh, sizetype vertexCount, sizetype indexCount, u32 creationFlags);

	V3f Mesh_GetPosition(Mesh* mesh, GLushort index);
	V3f Mesh_GetNormal(Mesh* mesh, GLushort index);
	V3f Mesh_GetPositionFromArray(Mesh* mesh, sizetype index);
	V3f Mesh_GetNormalFromArray(Mesh* mesh, sizetype index);

	void Mesh_SetDrawingIndex(Mesh* mesh,sizetype index, GLushort drawIndex);
	void Mesh_SetPositionToArray(Mesh* mesh,sizetype index, V3f position);
	void Mesh_SetNormalToArray(Mesh* mesh,sizetype index, V3f normal);
	void Mesh_SetUVToArray(Mesh* mesh,sizetype index, vec2 uv);
	bool Mesh_GetTriangleIndices(Mesh* mesh,GLsizei triangleIndex, GLushort* outA, GLushort* outB, GLushort* outC);

	void Mesh_DebugPrint(Mesh* mesh);


	void Mesh_SetupVertexArrays(Mesh* mesh);
	void Mesh_DrawElements(Mesh* mesh);
	// TODO void Mesh_DrawElementsPartially(Mesh* mesh, float start, float amount);
	void Mesh_DrawPoints(Mesh* mesh);
	void Mesh_DrawLines(Mesh* mesh);
	void Mesh_DrawNormals(Mesh* mesh);
	void Mesh_CalculateMatcapUVs(Mesh* mesh, MTX4x4 modelViewMatrix, MTX4x4 normalMatrix);

	// Setting vertices into arrays : returns the index of vertex in GLushort

	// SetPosition must be called first, just like in drawing
	GLushort Mesh_AddPosition(Mesh* mesh, V3f vertex);
	void Mesh_AddNormal(Mesh* mesh, V3f normal);
	void Mesh_AddUV(Mesh* mesh, vec2 normal);
	void Mesh_AddColor(Mesh* mesh, V3f color);
	u32 Mesh_AddTriangle(Mesh* mesh, GLushort indexA, GLushort indexB, GLushort indexC, u32 index);


	Mesh* Mesh_CreateIcosahedron(u32 creationFlags);
	Mesh* Mesh_CreateQuad(u32 creationFlags);
	Mesh* Mesh_CreateStar(float centerThickness, float pointRadius, float sharpness, int pointAmount, bool bothSides, u32 creationFlags);

	// Instant drawing
	void Mesh_DrawStarBorder(float thickness, float pointRadius, float sharpness, int pointAmount);
	Mesh* Mesh_CreateStarBorder(float borderThickness, float pointRadius, float sharpness, int pointAmount, u32 creationFlags);

	// TODO Mesh* Mesh_CreateRibbonPolygonCross(Mesh* bezierCurvePoints, int crossSectionPoints, float crossSectionRadius, int segmentsPerBezier);

	// TODO Mesh* Mesh_CreateRibbonMeshCross(Mesh* bezierCurvePoints, Mesh* crossSectionPoints, float crossSectionScale, int segmentsPerBezier);

	// TODO Mesh* Mesh_CreateCloud(float radius, int segments, float randomness);


#ifdef __cplusplus
}
#endif
