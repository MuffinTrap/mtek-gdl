#include <mgdl/mgdl-scene.h>

gdl::Material::Material()
{

}

gdl::Material::Material ( std::string name)
{
	this->name = name;
	this->shininess = 1.0f;
	this->emissiveColor = vec3New(0.0f, 0.0f, 0.0f);
	this->type = MaterialType::Diffuse;
}

gdl::Material::Material ( std::string name, gdl::Image* texture, gdl::MaterialType type)
{
	this->name = name;
	this->texture = texture;
	this->shininess = 1.0f;
	this->emissiveColor = vec3New(0.0f, 0.0f, 0.0f);
	this->type = type;
}



void gdl::Material::Apply()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->GetTextureId());
}
