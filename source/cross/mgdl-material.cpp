#include <mgdl/mgdl-scene.h>
#include <mgdl/mgdl-opengl_util.h>

static GLfloat whiteSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static GLfloat blackEmissive[4] = {0.0f, 0.0f, 0.0f, 1.0f};

Material* Material_Load (const char* name, Image* texture, MaterialType type)
{
	Material* material = (Material*)malloc(sizeof(Material));
	sizetype nameSize = strlen(name);
	material->name = new char[nameSize+1];
	strcpy(material->name, name);
	material->texture = texture;
	material->shininess = 1.0f;
	material->type = type;
	return material;
}

Material* Material_CreateColor(Color4f color, GLfloat shininess, GLfloat emissionPower)
{
	Material* material = (Material*)malloc(sizeof(Material));
	material->name = nullptr;
	material->texture = nullptr;
	material->shininess = shininess;
	material->diffuseColor[0] = color.red;
	material->diffuseColor[1] = color.green;
	material->diffuseColor[2] = color.blue;
	material->diffuseColor[3] = color.alpha;
	material->emissiveColor[0] = color.red * emissionPower;
	material->emissiveColor[1] = color.green * emissionPower;
	material->emissiveColor[2] = color.blue * emissionPower;
	material->emissiveColor[3] = color.alpha * emissionPower;
	material->type = MaterialType::Diffuse;
	return material;
}

void Material_Apply(Material* material)
{
	if (material->texture != nullptr)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, material->texture->textureId);
	}

	if( mgdl_GetLightingEnabled())
	{
		glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, material->diffuseColor);
		glMaterialf(GL_FRONT, GL_SHININESS, material->shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, material->emissiveColor);
	}
}

void Material_Reset(void)
{
	if( mgdl_GetLightingEnabled())
	{
		glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteSpecular);
		glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
		glMaterialfv(GL_FRONT, GL_EMISSION, blackEmissive);
	}
}
