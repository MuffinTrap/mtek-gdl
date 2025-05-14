#include <mgdl/mgdl-scene.h>
#include <mgdl/mgdl-logger.h>



sizetype Mesh_Init (Mesh* mesh, sizetype vertexCount, sizetype indexCount, bool createNormals, bool createUVs )
{
	mesh->positions = nullptr;
	mesh->indices = nullptr;
	mesh->normals = nullptr;
	mesh->uvs=nullptr;
	mesh->vertexCount = vertexCount;
	mesh->indexCount = indexCount;

	// Reserve space
	int byteCount = 0;
	mesh->indices = new GLushort[indexCount];
	byteCount += indexCount * sizeof(float);

	{
		// 3 floats per position
		sizetype positionFloats = vertexCount * 3;
		mesh->positions = new GLfloat[positionFloats];
		byteCount += positionFloats * sizeof(float);
	}

	if (createNormals)
	{
		// 3 floats per normal
		sizetype normalFloats = vertexCount * 3;
		mesh->normals = new GLfloat[normalFloats];
		byteCount += normalFloats * sizeof(float);
	}
	if (createUVs)
	{
		// 2 floats per uv
		sizetype uvFloats = vertexCount * 2;
		mesh->uvs = new GLfloat[uvFloats];
		byteCount += uvFloats * sizeof(float);
	}
	return byteCount;
}


void Mesh_SetupVertexArrays(Mesh* mesh)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, mesh->positions);
	if (mesh->normals != nullptr)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, mesh->normals);
	}
	if (mesh->uvs != nullptr)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, mesh->uvs);
	}
}

void Mesh_DrawElements(Mesh* mesh)
{
	Mesh_SetupVertexArrays(mesh);
	// NOTE OpenGX does not impelement glDrawRangeElements()
	//glDrawRangeElements(GL_TRIANGLES, 0, vertexCount-1, indexCount, GL_UNSIGNED_SHORT, indices);
	glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_SHORT, mesh->indices);
}

void Mesh_DrawPoints(Mesh* mesh)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, mesh->positions);
	glDrawElements(GL_POINTS, mesh->indexCount, GL_UNSIGNED_SHORT, mesh->indices);
}

void Mesh_DrawLines(Mesh* mesh)
{
	glDisableClientState(GL_VERTEX_ARRAY);
	for (GLsizei i = 0; i < mesh->indexCount; i+=3)
	{
		vec3 a = Mesh_GetPosition(mesh, i);
		vec3 b = Mesh_GetPosition(mesh, i+1);
		vec3 c = Mesh_GetPosition(mesh, i+2);
		glBegin(GL_LINE_LOOP);
			glVertex3f(a.x, a.y, a.z);
			glVertex3f(b.x, b.y, b.z);
			glVertex3f(c.x, c.y, c.z);
		glEnd();
	}
}

void Mesh_DrawNormals(Mesh* mesh)
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glBegin(GL_LINES);
	for (GLsizei i = 0; i < mesh->indexCount; i++)
	{
		vec3 n = Mesh_GetNormal(mesh, i);
		vec3 a = Mesh_GetPosition(mesh, i);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(a.x + n.x, a.y + n.y, a.z + n.z);
	}
	glEnd();
}

void Mesh_CalculateMatcapUVs(Mesh* mesh,const mat4x4& modelViewMatrix, const mat4x4& normalMatrix)
{
	// This calculation happens in screen space

	vec3 eye;
	vec3 normal; // screen space normalo
	vec4 normal4;
	vec3 reflection;
	vec2 R2;
	vec3 position;
	vec4 position4;
	vec2 matcapUV;

	// Debug print
	static bool once = false;
	// Overwrite UVs
	for (sizetype i = 0; i < mesh->vertexCount; i++)
	{
		position = Mesh_GetPositionFromArray(mesh, i);
		normal = Mesh_GetNormalFromArray(mesh, i);
	}

	const vec2 half = vec2New(0.5f, 0.5f);
	for (sizetype i = 0; i < mesh->vertexCount; i++)
	{
		position = Mesh_GetPositionFromArray(mesh, i);
		normal = Mesh_GetNormalFromArray(mesh, i);

		normal4 = vec4New(normal.x, normal.y, normal.z, 0.0f);
		position4 = vec4New(position.x, position.y, position.z, 1.0f);
		position4 = mat4x4MultiplyVector(modelViewMatrix, position4);
		position = position4.xyz;
		matcapUV = vec2New(0.5f, 0.5f);
		if (vec3Length(position) != 0.0f)
		{
			eye = vec3Normalize(position);
			normal4 = mat4x4MultiplyVector(normalMatrix, normal4);
			normal = vec3Normalize(normal4.xyz);

			reflection= vec3Reflect(normal, eye);// Reflect eye with normal

			const float rx2 = pow(reflection.x, 2.0f);
			const float ry2 = pow(reflection.y, 2.0f);
			const float rz12 = pow(reflection.z+1, 2.0f);
			const float sqrtR2 = sqrt(rx2 + ry2 + rz12) * 2.0f;
			R2 = reflection.xy;
			matcapUV = vec2Add(vec2New(R2.x/sqrtR2, R2.y/sqrtR2), half);
		}
		Mesh_SetUVToArray(mesh, i, matcapUV);
	}
	once = false;
}

void Mesh_SetPositionToArray(Mesh* mesh, sizetype index, const vec3& position )
{
	if (index < mesh->vertexCount)
	{
		sizetype i = index * 3;
		mesh->positions[i+0] = position.x;
		mesh->positions[i+1] = position.y;
		mesh->positions[i+2] = position.z;
	}
}


// This is a drawing index, not an array index
vec3 Mesh_GetPosition ( Mesh* mesh,GLushort index )
{
	if (index < mesh->indexCount)
	{
		// What vertex is drawn when index
		GLushort position = mesh->indices[index];
		// Get the index to float array
		sizetype i = position * 3;
		return V3f_Create(mesh->positions[i+0], mesh->positions[i+1], mesh->positions[i+2]);
	}
	Log_ErrorF("No such index! %d > %d\n", index, mesh->indexCount);
	return V3f_Create(0.0f, 0.0f, 0.0f);
}

vec3 Mesh_GetNormal (Mesh* mesh, GLushort index )
{
	if (index < mesh->indexCount)
	{
		// What vertex is drawn when index
		GLushort position = mesh->indices[index];
		// Get the index to float array
		sizetype i = position * 3;
		return V3f_Create(mesh->normals[i+0], mesh->normals[i+1], mesh->normals[i+2]);
	}
	Log_ErrorF("No such index! %d > %d\n", index, mesh->indexCount);
	return V3f_Create(0.0f, 0.0f, 0.0f);
}


void Mesh_SetNormalToArray ( Mesh* mesh,sizetype index, const vec3& normal )
{
	if (index < mesh->vertexCount)
	{
		sizetype vi = index * 3;
		mesh->normals[vi+0] = normal.x;
		mesh->normals[vi+1] = normal.y;
		mesh->normals[vi+2] = normal.z;
	}
}

bool Mesh_GetTriangleIndices (Mesh* mesh, GLsizei triangleIndex, GLushort& outA, GLushort& outB, GLushort& outC )
{
	GLsizei indice = triangleIndex * 3;
	if (indice + 2 < mesh->indexCount)
	{
		outA = mesh->indices[indice];
		outB = mesh->indices[indice+1];
		outC = mesh->indices[indice+2];
		return true;
	}
	return false;
}

void Mesh_SetUVToArray (Mesh* mesh, sizetype index, const vec2& uv )
{
	if (mesh->uvs != nullptr)
	{
		sizetype vi = index * 2;
		mesh->uvs[vi + 0] = uv.x;
		mesh->uvs[vi + 1] = uv.y;
	}
}

void Mesh_SetDrawingIndex ( Mesh* mesh,sizetype index, GLushort drawIndex )
{
	mesh->indices[index] = drawIndex;
}


vec3 Mesh_GetPositionFromArray(Mesh* mesh,sizetype index)
{
	if (index < mesh->vertexCount)
	{
		sizetype vi = index * 3;
		return V3f_Create(mesh->positions[vi+0], mesh->positions[vi+1], mesh->positions[vi+2]);
	}
	Log_ErrorF("index %zu > %u vertexCount!\n", index, mesh->vertexCount);
	return V3f_Create(0.0f, 0.0f, 0.0f);
}
vec3 Mesh_GetNormalFromArray(Mesh* mesh,sizetype index)
{
	if (index < mesh->vertexCount)
	{
		sizetype vi = index * 3;
		return V3f_Create(mesh->normals[vi+0], mesh->normals[vi+1], mesh->normals[vi+2]);
	}
	Log_ErrorF("index %zu > %u vertexCount!\n", index, mesh->vertexCount);
	return V3f_Create(0.0f, 1.0f, 0.0f);
}

// Mesh creation functions

Mesh* Mesh_CreateIcosahedron(bool normals, bool uvs)
{
    const float X = 0.525731112119133606;
    const float Z = 0.850650808352039932;
    const float N = 0.0f;

	Mesh* icosa = new Mesh();
	Mesh_Init(icosa, 12, 60, normals, uvs);

	delete[] icosa->positions;
    icosa->positions = new GLfloat[12*3] {
		-X, N, Z,
		 X,  N,  Z,
		-X,  N, -Z,
		 X,  N, -Z,
		 N,  Z,  X,
		 N,  Z, -X,
		 N, -Z,  X,
		 N, -Z, -X,
		 Z,  X,  N,
		-Z,  X,  N,
		 Z, -X,  N,
		-Z, -X,  N};

    if (normals)
	{
		delete[] icosa->normals;
		icosa->normals = new GLfloat[12*3] {
			-X, N, Z,
			X,  N,  Z,
			-X,  N, -Z,
			X,  N, -Z,
			N,  Z,  X,
			N,  Z, -X,
			N, -Z,  X,
			N, -Z, -X,
			Z,  X,  N,
			-Z,  X,  N,
			Z, -X,  N,
			-Z, -X,  N};
	}

    // OpenGL books the indices in CC winding

    delete[] icosa->indices;
	icosa->indices = new GLushort[60] {
		1,4,0,  4,9,0,  4,5,9,  8,5,4, 1,8,4,  // 15
		1,10,8, 10,3,8, 8,3,5, 3,2,5,  3,7,2,  // 30
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,  // 45
		10,1,6, 11,0,9,  2,11,9, 5,2,9, 11,2,7   // 60
		};

	icosa->indexCount = 60;

	icosa->name = "Icosahedron";


    return icosa;
}

void Mesh_DebugPrint(Mesh* mesh)
{
	printf("%s mesh has %u vertices and %u indices\n", mesh->name, mesh->vertexCount, mesh->indexCount);
	for (sizetype i = 0; i < mesh->vertexCount; i++)
	{
		sizetype vi = i*3;
		printf("%zu: Pos %.2f, %.2f, %.2f\n", i, mesh->positions[vi+0], mesh->positions[vi+1], mesh->positions[vi+2]);
	}
	printf("\n");
	for (sizetype i = 0; i < mesh->vertexCount; i++)
	{
		vec3 pos = Mesh_GetPositionFromArray(mesh, i);
		vec3 normal = Mesh_GetNormalFromArray(mesh, i);
		printf("%zu: Pos %.2f, %.2f, %.2f\tN %.2f, %.2f, %.2f\n", i, pos.x, pos.y, pos.z, normal.x, normal.y, normal.z);
	}
}


Mesh * Mesh_CreateQuad ( bool normals, bool uvs )
{
	Mesh* quad = new Mesh();
	Mesh_Init(quad, 4, 6, normals, uvs);
	delete[] quad->positions;

	float sz = 0.5f;
    quad->positions = new GLfloat[4*3] {
		-sz, -sz, 0.0f,
		sz, -sz, 0.0f,
		sz, sz, 0.0f,
		-sz, sz, 0.0f};

	for (int i = 0; i < 4*3; i+=3)
	{
		quad->normals[i+0] = 0.0f;
		quad->normals[i+1] = 0.0f;
		quad->normals[i+2] = 1.0f;
	}

	Mesh_SetUVToArray(quad, 0, vec2New(0.0f, 0.0f));
	Mesh_SetUVToArray(quad, 1, vec2New(1.0f, 0.0f));
	Mesh_SetUVToArray(quad, 2, vec2New(1.0f, 1.0f));
	Mesh_SetUVToArray(quad, 3, vec2New(0.0f, 1.0f));

	quad->indices[0] = 0;
	quad->indices[1] = 1;
	quad->indices[2] = 2;
	quad->indices[3] = 2;
	quad->indices[4] = 3;
	quad->indices[5] = 0;

	quad->name = "Quad";

	return quad;
}





