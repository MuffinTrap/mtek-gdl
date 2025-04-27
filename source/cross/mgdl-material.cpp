#include <mgdl/mgdl-scene.h>

void Material_Init (Material* material, const char* name, Image* texture, gdl::MaterialType type)
{
	material->name = name;
	material->texture = texture;
	material->shininess = 1.0f;
	material->emissiveColor = vec3New(0.0f, 0.0f, 0.0f);
	material->type = type;
}

void Material_Apply(Material* material)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, material->texture->textureId);
}
