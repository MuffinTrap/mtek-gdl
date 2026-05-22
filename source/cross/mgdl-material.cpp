#include <mgdl/mgdl-scene.h>
#include <mgdl/mgdl-opengl_util.h>
#include <mgdl/mgdl-memory.h>
#include <cstring>

static GLfloat whiteSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static GLfloat blackEmissive[4] = {0.0f, 0.0f, 0.0f, 1.0f};

Material* Material_Load (const char* name, Texture* texture, MaterialType type)
{
	Material* material = (Material*)mgdl_AllocateGraphicsMemory(sizeof(Material));
	sizetype nameSize = strlen(name);
	material->name = new char[nameSize+1];
	strncpy(material->name,  name, nameSize+1);
	material->texture = texture;
	material->shininess = 1.0f;
	material->type = type;
	return material;
}

Material* Material_CreateColor(color32 color, GLfloat shininess)
{
	Material* material = (Material*)mgdl_AllocateGraphicsMemory(sizeof(Material));
	material->name = nullptr;
	material->texture = nullptr;
	material->shininess = shininess;
	Color_HexToGLfloats(color, material->diffuseColor);
	Color_HexToGLfloats(color, material->emissiveColor);
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
void Material_SetDiffuseColor(Material* material, color32 color)
{
	Color_HexToGLfloats(color, material->diffuseColor);
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
void Material_Free(Material* m)
{
	if (m != nullptr)
	{
		mgdl_FreeGraphicsMemory(m);
		m = nullptr;
	}
}
