#include <mgdl/mgdl-scene.h>

Material* Material_Load (const char* name, Image* texture, MaterialType type)
{
	Material* material = new Material();
	sizetype nameSize = strlen(name);
	material->name = new char[nameSize+1];
	strcpy(material->name, name);
	material->texture = texture;
	material->shininess = 1.0f;
	material->emissiveColor = V3f_Create(0.0f, 0.0f, 0.0f);
	material->type = type;
	return material;
}

void Material_Apply(Material* material)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, material->texture->textureId);
}
