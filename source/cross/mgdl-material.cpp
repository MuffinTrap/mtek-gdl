#include <mgdl/mgdl-scene.h>

void gdl::Material::Apply()
{
	if (texture != nullptr)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureId());
	}
	else{
		printf("Material has no texture!\n");
	}
}
