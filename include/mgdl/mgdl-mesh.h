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
	uint32_t uniqueId;
};

vec3 Mesh_GetPosition(Mesh* mesh,GLushort index);
vec3 Mesh_GetNormal(Mesh* mesh,GLushort index);
vec3 Mesh_GetPositionFromArray(Mesh* mesh,sizetype index);
vec3 Mesh_GetNormalFromArray(Mesh* mesh,sizetype index);
void Mesh_SetDrawingIndex(Mesh* mesh,sizetype index, GLushort drawIndex);
void Mesh_SetPositionToArray(Mesh* mesh,sizetype index, const vec3& position);
void Mesh_SetNormalToArray(Mesh* mesh,sizetype index, const vec3& normal);
void Mesh_SetUVToArray(Mesh* mesh,sizetype index, const vec2& uv);
bool Mesh_GetTriangleIndices(Mesh* mesh,GLsizei triangleIndex, GLushort& outA, GLushort& outB, GLushort& outC);
void Mesh_DebugPrint(Mesh* mesh);

// Returns the amount of bytes reserved
sizetype Mesh_Init(Mesh* mesh, sizetype vertexCount, sizetype indexCount, bool createNormals, bool createUVs);

void Mesh_SetupVertexArrays(Mesh* mesh);
void Mesh_DrawElements(Mesh* mesh);
void Mesh_DrawPoints(Mesh* mesh);
void Mesh_DrawLines(Mesh* mesh);
void Mesh_DrawNormals(Mesh* mesh);
void Mesh_CalculateMatcapUVs(Mesh* mesh,const mat4x4& modelViewMatrix, const mat4x4& normalMatrix);

Mesh* Mesh_CreateIcosahedron(bool normals, bool uvs);
Mesh* Mesh_CreateQuad(bool normals, bool uvs);

