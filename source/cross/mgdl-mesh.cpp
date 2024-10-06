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
	glDrawRangeElements(GL_TRIANGLES, 0, vertexCount-1, indexCount, GL_UNSIGNED_SHORT, indices);
}

