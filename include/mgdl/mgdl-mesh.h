#pragma once

#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-types.h>

/**
 * @file mgdl-mesh.h
 * @ingroup draw3d scene
 * @brief Mesh class for storing 3D models
 */

/* Contains vertex data that is needed for
 * rendering the mesh
 * positions, normals, uvs, indices
 */
struct Mesh
{
	// OpenGL data
	GLushort* indices;
	GLsizei indexCount;
	u32 vertexCount;
	GLfloat* positions;
	GLfloat* normals;
	GLfloat* uvs;
	GLfloat* colors;

	// mgdl data
	GLushort indexCounter; /**< Used when creating a mesh */
	zstr name;

	// ufbx connection
	uint32_t ufbx_id;
};
typedef struct Mesh Mesh;


#ifdef __cplusplus
extern "C"
{
#endif
	Mesh* Mesh_CreateEmpty(void);
	/**
	 * @brief Reserves space for a mesh and sets mesh variables.
	 * @details Use this function to reserve memory for a mesh. After this the Mesh_Add* functions can be called.
	 * @param mesh The mesh to initialize.
	 * @param vertexCount How many vertices the mesh will have.
	 * @param indexCount How many indices the mesh will have.
	 * @param createFlags Collection of flags specifying what vertex attributes to create
	 * @return Amount of bytes allocated.
	 */
	sizetype Mesh_Init(Mesh* mesh, sizetype vertexCount, sizetype indexCount, u32 creationFlags);

	/**
	 * @brief Get a vertex position by drawing index
	 * @param mesh The mesh
	 * @param index The drawing index for the vertex.
	 * @returns The vertex position or zero if index is invalid.
	 */
	Vector3 Mesh_GetPosition(Mesh* mesh, GLushort index);
	/**
	 * @brief Get a vertex normal by drawing index
	 * @param mesh The mesh
	 * @param index The drawing index for the vertex.
	 * @returns The vertex normal or zero if index is invalid.
	 */
	Vector3 Mesh_GetNormal(Mesh* mesh, GLushort index);
	/**
	 * @brief Get a vertex position by array index
	 * @param mesh The mesh
	 * @param index The array index for the vertex.
	 * @returns The vertex position or zero if index is invalid.
	 */
	Vector3 Mesh_GetPositionFromArray(Mesh* mesh, sizetype index);
	/**
	 * @brief Get a vertex normal by array index
	 * @param mesh The mesh
	 * @param index The array index for the vertex.
	 * @returns The vertex normal or zero if index is invalid.
	 */
	Vector3 Mesh_GetNormalFromArray(Mesh* mesh, sizetype index);

	/**
	 * @brief Sets an index in the indices array
	 * @param mesh The mesh
	 * @param index The indices array index
	 * @param drawIndex The drawing index
	 */
	void Mesh_SetDrawingIndex(Mesh* mesh,sizetype index, GLushort drawIndex);
	/**
	 * @brief Sets a vertex position in the positions array
	 * @param mesh The mesh
	 * @param index The positions array index
	 * @param position The vertex position
	 */
	void Mesh_SetPositionToArray(Mesh* mesh,sizetype index, Vector3 position);
		/**
	 * @brief Sets a vertex normal in the normals array
	 * @param mesh The mesh
	 * @param index The normals array index
	 * @param normal The vertex normal
	 */
	void Mesh_SetNormalToArray(Mesh* mesh,sizetype index, Vector3 normal);
		/**
	 * @brief Sets a vertex uv in the texture coordinates array
	 * @param mesh The mesh
	 * @param index The texture coordinates array index
	 * @param uv The vertex uv
	 */
	void Mesh_SetUVToArray(Mesh* mesh,sizetype index, Vector2 uv);
		/**
	 * @brief Gets drawing indices of a given vertext triangle
	 * @param mesh The mesh
	 * @param triangleIndex The index of the triangle in the positions array
	 * @param[out] outA Pointer for storing the first index
	 * @param[out] outB Pointer for storing the second index
	 * @param[out] outC Pointer for storing the third index
	 * @returns True if something sensible was written to out parameters.
	 */
	bool Mesh_GetTriangleIndices(Mesh* mesh,GLsizei triangleIndex, GLushort* outA, GLushort* outB, GLushort* outC);

	/**
	 * @brief Prints information about a mesh
	 * @param mesh The mesh
	 */
	void Mesh_DebugPrint(Mesh* mesh);


	/**
	 * @brief Sets the pointers to mesh data to OpenGL
	 * @note This is called by drawing functions.
	 */
	void Mesh_SetupVertexArrays(Mesh* mesh);

	/**
	 * @brief Draws the mesh partially
	 * @param mesh The mesh
	 * @param indexCount How many indices to use for drawing. This is automatically limited to mesh's index indexCount
	 */
	void Mesh_DrawElements(Mesh* mesh, GLsizei indexCount);

	/**
	 * @brief Draws the mesh
	 * @param mesh The mesh to draw
	 */
	void Mesh_DrawArrays(Mesh* mesh);
	// TODO void Mesh_DrawElementsPartially(Mesh* mesh, float start, float amount);

	/**
	 * @brief Draw the mesh vertices as points
	 * @param mesh Mesh
	 */
	void Mesh_DrawPoints(Mesh* mesh);
	/**
	 * @brief Draw the mesh using lines
	 * @param mesh The mesh
	 */
	void Mesh_DrawLines(Mesh* mesh);
	/**
	 * @brief Draws the normals of the mesh as lines
	 * @param mesh The mesh
	 */
	void Mesh_DrawNormals(Mesh* mesh);

	/**
	 * @brief Calculates new texture coordinates to use with a matcap texture
	 * @note This is called automatically by Model when using a matcap material.
	 * @param mesh The mesh
	 * @param modelViewMatrix The model-view matrix of the mesh
	 * @param normalMatrix The matrix for normals
	 */
	void Mesh_CalculateMatcapUVs(Mesh* mesh, Matrix modelViewMatrix, Matrix normalMatrix);

	/**
	 * @brief Adds a position to array of vertex positions.
	 * @note Call this before adding other vertex data
	 * @param mesh The mesh
	 * @param vertex The vertex position
	 * @returns The index where the position was set
	 */
	GLushort Mesh_AddPosition(Mesh* mesh, Vector3 vertex);
	/**
	 * @brief Adds a normal to array of vertex normals.
	 * @note This will attach the normal to previously added position.
	 * @param mesh The mesh
	 * @param vertex The vertex normal
	 */
	void Mesh_AddNormal(Mesh* mesh, Vector3 normal);
	/**
	 * @brief Adds a uv to array of vertex texture coordinates.
	 * @note This will attach the uv to previously added position.
	 * @param mesh The mesh
	 * @param vertex The vertex uv
	 */
	void Mesh_AddUV(Mesh* mesh, Vector2 uv);
		/**
	 * @brief Adds a color to array of vertex colors.
	 * @note This will attach the color to previously added position.
	 * @param mesh The mesh
	 * @param vertex The vertex color
	 */
	void Mesh_AddColor(Mesh* mesh, Vector3 color);

	/**
	 * @brief Sets drawing indices to draw a triangle
	 * @param mesh The mesh
	 * @param indexA The first drawing index, referring to positions array.
	 * @param indexB The second drawing index, referring to positions array.
	 * @param indexC The third drawing index, referring to positions array.
	 * @param index The index of the triangle. Use 0 as the first index.
	 * @returns The next index to use.
	 */
	u32 Mesh_AddTriangle(Mesh* mesh, GLushort indexA, GLushort indexB, GLushort indexC, u32 index);


	/**
	 * @brief Creates an icosahedron mesh
	 * @param creationFlags Flags that tell if to create normals, uvs and colors
	 * @returns Pointer to the created mesh
	 */
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
