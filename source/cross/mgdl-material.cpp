#include <mgdl/mgdl-scene.h>

gdl::Material::Material()
{

}

gdl::Material::Material ( std::string name)
{
	this->name = name;
	this->shininess = 1.0f;
	this->emissiveColor = gdl::vec3(0.0f, 0.0f, 0.0f);
}

gdl::Material::Material ( std::string name, gdl::Image* texture )
{
	this->name = name;
	this->texture = texture;
	this->shininess = 1.0f;
	this->emissiveColor = gdl::vec3(0.0f, 0.0f, 0.0f);
}



void gdl::Material::Apply()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->GetTextureId());
}
