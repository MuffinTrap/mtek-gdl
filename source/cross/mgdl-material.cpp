#include <mgdl/mgdl-scene.h>

void gdl::Material::Apply()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->GetTextureId());
}
