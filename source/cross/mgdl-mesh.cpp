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
		gdl::vec3 a = GetPosition(i);
		gdl::vec3 b = GetPosition(i+1);
		gdl::vec3 c = GetPosition(i+2);
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
		gdl::vec3 n = GetNormal(i);
		gdl::vec3 a = GetPosition(i);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(a.x + n.x, a.y + n.y, a.z + n.z);
	}
	glEnd();
}


// This is a drawing index, not an array index
gdl::vec3 gdl::Mesh::GetPosition ( GLushort index )
{
	if (index < indexCount)
	{
		// What vertex is drawn when index
		GLushort position = indices[index];
		// Get the index to float array
		size_t i = position * 3;
		return gdl::vec3(positions[i+0], positions[i+1], positions[i+2]);
	}
	printf("No such index! %d > %d\n", index, indexCount);
	return gdl::vec3(0.0f, 0.0f, 0.0f);
}

gdl::vec3 gdl::Mesh::GetNormal ( GLushort index )
{
	if (index < indexCount)
	{
		// What vertex is drawn when index
		GLushort position = indices[index];
		// Get the index to float array
		size_t i = position * 3;
		return gdl::vec3(normals[i+0], normals[i+1], normals[i+2]);
	}
	printf("No such index! %d > %d\n", index, indexCount);
	return gdl::vec3(0.0f, 0.0f, 0.0f);
}


void gdl::Mesh::SetNormal ( GLsizei index, const gdl::vec3& normal )
{
	if (index < indexCount)
	{
		size_t i = index * 3;
		normals[i+0] = normal.x;
		normals[i+1] = normal.y;
		normals[i+2] = normal.z;
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





