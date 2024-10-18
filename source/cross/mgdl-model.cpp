#include <mgdl/mgdl-scene.h>

void gdl::Model::AddMesh(gdl::Mesh* meshIn)
{
	meshes.push_back(meshIn);
}
void gdl::Model::AddTexturedMesh ( gdl::Mesh* meshIn, gdl::Image* imageIn )
{
	AddMesh(meshIn);
	AddTexture(imageIn);
}

void gdl::Model::AddTexture ( gdl::Image* imageIn )
{
	textures.push_back(imageIn);
}

void gdl::Model::Draw ( int texture )
{
	glEnable(GL_TEXTURE_2D);
	if (texture >= 0 && texture < (int)textures.size())
	{
		glBindTexture(GL_TEXTURE_2D, textures[texture]->GetTextureId());
	}
	for(size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i]->DrawElements();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}


