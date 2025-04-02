#include <mgdl/mgdl-scene.h>

gdl::Mesh::Mesh()
{
	positions = nullptr;
	vertexCount = 0;
	indices = nullptr;
	indexCount = 0;
	normals = nullptr;
	uvs=nullptr;
}

sizetype gdl::Mesh::Allocate ( sizetype vertexCount, sizetype indexCount, bool createNormals, bool createUVs )
{
	this->vertexCount = vertexCount;
	this->indexCount = indexCount;

	// Reserve space
	int byteCount = 0;
	indices = new GLushort[indexCount];
	byteCount += indexCount * sizeof(float);

	{
		// 3 floats per position
		sizetype positionFloats = vertexCount * 3;
		positions = new GLfloat[positionFloats];
		byteCount += positionFloats * sizeof(float);
	}

	if (createNormals)
	{
		// 3 floats per normal
		sizetype normalFloats = vertexCount * 3;
		normals = new GLfloat[normalFloats];
		byteCount += normalFloats * sizeof(float);
	}
	if (createUVs)
	{
		// 2 floats per uv
		sizetype uvFloats = vertexCount * 2;
		uvs = new GLfloat[uvFloats];
		byteCount += uvFloats * sizeof(float);
	}
	return byteCount;
}


void gdl::Mesh::SetupVertexArrays()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, positions);
	if (normals != nullptr)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, normals);
	}
	if (uvs != nullptr)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
	}
}

void gdl::Mesh::DrawElements()
{
	SetupVertexArrays();
	// NOTE OpenGX does not impelement glDrawRangeElements()
	//glDrawRangeElements(GL_TRIANGLES, 0, vertexCount-1, indexCount, GL_UNSIGNED_SHORT, indices);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, indices);
}

void gdl::Mesh::DrawPoints()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, positions);
	glDrawElements(GL_POINTS, indexCount, GL_UNSIGNED_SHORT, indices);
}

void gdl::Mesh::DrawLines()
{
	glDisableClientState(GL_VERTEX_ARRAY);
	for (GLsizei i = 0; i < indexCount; i+=3)
	{
		vec3 a = GetPosition(i);
		vec3 b = GetPosition(i+1);
		vec3 c = GetPosition(i+2);
		glBegin(GL_LINE_LOOP);
			glVertex3f(a.x, a.y, a.z);
			glVertex3f(b.x, b.y, b.z);
			glVertex3f(c.x, c.y, c.z);
		glEnd();
	}
}

void gdl::Mesh::DrawNormals()
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glBegin(GL_LINES);
	for (GLsizei i = 0; i < indexCount; i++)
	{
		vec3 n = GetNormal(i);
		vec3 a = GetPosition(i);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(a.x + n.x, a.y + n.y, a.z + n.z);
	}
	glEnd();
}

void gdl::Mesh::CalculateMatcapUVs(const mat4x4& modelViewMatrix, const mat4x4& normalMatrix)
{
	// This calculation happens in screen space

	vec3 eye;
	vec4 eye4; //from camera to vertex
	vec3 normal; // screen space normalo
	vec4 normal4;
	vec3 reflection;
	vec2 R2;
	vec3 position;
	vec4 position4;
	vec2 matcapUV;

	static bool once = true;
	// Overwrite UVs
	for (sizetype i = 0; i < vertexCount; i++)
	{
		position = GetPositionFromArray(i);
		normal = GetNormalFromArray(i);

	if (once)
	{
			printf("Before matrices:\n");
	}
		if (once)
		{
			printf("%zu: Pos %.2f, %.2f, %.2f\tN %.2f, %.2f, %.2f ->\tUV %.2f, %.2f\n", i,
				   position.x, position.y, position.z,
		  normal.x, normal.y, normal.z,
		  matcapUV.x, matcapUV.y);
		}
	}

	if (once)
	{
		printf("After matrices:\n");
	}
	const vec2 half = vec2New(0.5f, 0.5f);
	for (sizetype i = 0; i < vertexCount; i++)
	{
		position = GetPositionFromArray(i);
		normal = GetNormalFromArray(i);

		normal4 = vec4New(normal.x, normal.y, normal.z, 0.0f);
		position4 = vec4New(position.x, position.y, position.z, 1.0f);
		position4 = mat4x4MultiplyVector(modelViewMatrix, position4);
		position = position4.xyz;
		eye = vec3Normalize(position);
		normal4 = mat4x4MultiplyVector(normalMatrix, normal4);
		normal = vec3Normalize(normal4.xyz);

		matcapUV = vec2New(0.5f, 0.5f);
		reflection= vec3Reflect(normal, eye);// Reflect eye with normal

		const float rx2 = pow(reflection.x, 2.0f);
		const float ry2 = pow(reflection.y, 2.0f);
		const float rz12 = pow(reflection.z+1, 2.0f);
		const float sqrtR2 = sqrt(rx2 + ry2 + rz12) * 2.0f;
		R2 = reflection.xy;
		matcapUV = vec2Add(vec2New(R2.x/sqrtR2, R2.y/sqrtR2), half);
		if (once)
		{
			printf("%zu: Pos %.2f, %.2f, %.2f\tN %.2f, %.2f, %.2f ->\tUV %.2f, %.2f\n", i,
				   eye.x, eye.y, eye.z,
		  normal.x, normal.y, normal.z,
		  matcapUV.x, matcapUV.y);
		}
		SetUVToArray(i, matcapUV);
	}
	once = false;
}

void gdl::Mesh::SetPositionToArray( sizetype index, const vec3& position )
{
	if (index < vertexCount)
	{
		sizetype i = index * 3;
		positions[i+0] = position.x;
		positions[i+1] = position.y;
		positions[i+2] = position.z;
	}
}


// This is a drawing index, not an array index
vec3 gdl::Mesh::GetPosition ( GLushort index )
{
	if (index < indexCount)
	{
		// What vertex is drawn when index
		GLushort position = indices[index];
		// Get the index to float array
		sizetype i = position * 3;
		return vec3New(positions[i+0], positions[i+1], positions[i+2]);
	}
	printf("No such index! %d > %d\n", index, indexCount);
	return vec3New(0.0f, 0.0f, 0.0f);
}

vec3 gdl::Mesh::GetNormal ( GLushort index )
{
	if (index < indexCount)
	{
		// What vertex is drawn when index
		GLushort position = indices[index];
		// Get the index to float array
		sizetype i = position * 3;
		return vec3New(normals[i+0], normals[i+1], normals[i+2]);
	}
	printf("No such index! %d > %d\n", index, indexCount);
	return vec3New(0.0f, 0.0f, 0.0f);
}


void gdl::Mesh::SetNormalToArray ( sizetype index, const vec3& normal )
{
	if (index < vertexCount)
	{
		sizetype vi = index * 3;
		normals[vi+0] = normal.x;
		normals[vi+1] = normal.y;
		normals[vi+2] = normal.z;
	}
}

bool gdl::Mesh::GetTriangleIndices ( GLsizei triangleIndex, GLushort& outA, GLushort& outB, GLushort& outC )
{
	GLsizei indice = triangleIndex * 3;
	if (indice + 2 < indexCount)
	{
		outA = indices[indice];
		outB = indices[indice+1];
		outC = indices[indice+2];
		return true;
	}
	return false;
}

void gdl::Mesh::SetUVToArray ( sizetype index, const vec2& uv )
{
	if (uvs != nullptr)
	{
		sizetype vi = index * 2;
		uvs[vi + 0] = uv.x;
		uvs[vi + 1] = uv.y;
	}
}

void gdl::Mesh::SetDrawingIndex ( sizetype index, GLushort drawIndex )
{
	indices[index] = drawIndex;
}


vec3 gdl::Mesh::GetPositionFromArray(sizetype index)
{
	if (index < vertexCount)
	{
		sizetype vi = index * 3;
		return vec3New(positions[vi+0], positions[vi+1], positions[vi+2]);
	}
	printf("index %zu > %u vertexCount!\n", index, vertexCount);
	return vec3New(0.0f, 0.0f, 0.0f);
}
vec3 gdl::Mesh::GetNormalFromArray(sizetype index)
{
	if (index < vertexCount)
	{
		sizetype vi = index * 3;
		return vec3New(normals[vi+0], normals[vi+1], normals[vi+2]);
	}
	printf("index %zu > %u vertexCount!\n", index, vertexCount);
	return vec3New(0.0f, 1.0f, 0.0f);
}

// Mesh creation functions

gdl::Mesh* gdl::Mesh::CreateIcosahedron(bool normals, bool uvs)
{
    const float X = 0.525731112119133606;
    const float Z = 0.850650808352039932;
    const float N = 0.0f;

	Mesh* icosa = new Mesh();
	icosa->Allocate(12, 60, normals, uvs);

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

void gdl::Mesh::DebugPrint()
{
	printf("%s mesh has %u vertices and %u indices\n", name.c_str(), vertexCount, indexCount);
	for (sizetype i = 0; i < vertexCount; i++)
	{
		sizetype vi = i*3;
		printf("%zu: Pos %.2f, %.2f, %.2f\n", i, positions[vi+0], positions[vi+1], positions[vi+2]);
	}
	printf("\n");
	for (sizetype i = 0; i < vertexCount; i++)
	{
		vec3 pos = GetPositionFromArray(i);
		vec3 normal = GetNormalFromArray(i);
		printf("%zu: Pos %.2f, %.2f, %.2f\tN %.2f, %.2f, %.2f\n", i, pos.x, pos.y, pos.z, normal.x, normal.y, normal.z);
	}
}


gdl::Mesh * gdl::Mesh::CreateQuad ( bool normals, bool uvs )
{
	gdl::Mesh* quad = new Mesh();
	quad->Allocate(4, 6, normals, uvs);
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

	quad->SetUVToArray(0, vec2New(0.0f, 0.0f));
	quad->SetUVToArray(1, vec2New(1.0f, 0.0f));
	quad->SetUVToArray(2, vec2New(1.0f, 1.0f));
	quad->SetUVToArray(3, vec2New(0.0f, 1.0f));

	quad->indices[0] = 0;
	quad->indices[1] = 1;
	quad->indices[2] = 2;
	quad->indices[3] = 2;
	quad->indices[4] = 3;
	quad->indices[5] = 0;

	quad->name = "Quad";

	return quad;
}





