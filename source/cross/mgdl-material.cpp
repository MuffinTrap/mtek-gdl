#include <mgdl/mgdl-scene.h>
#include <mgdl/mgdl-opengl_util.h>
#include <mgdl/mgdl-memory.h>
#include <cstring>

static GLfloat whiteSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static GLfloat blackEmissive[4] = {0.0f, 0.0f, 0.0f, 1.0f};

Material* Material_Load (const char* name, Texture* texture, MaterialType type)
{
	Material* material = (Material*)mgdl_AllocateGraphicsMemory(sizeof(Material));
	material->name = zstr_from(name);
	material->texture = texture;
	material->shininess = 1.0f;
	material->type = type;
	material->diffuseColor[0] = 1.0f;
	material->diffuseColor[1] = 1.0f;
	material->diffuseColor[2] = 1.0f;
	material->diffuseColor[3] = 1.0f;
	material->emissiveColor[0] = 0.0f;
	material->emissiveColor[1] = 0.0f;
	material->emissiveColor[2] = 0.0f;
	material->emissiveColor[3] = 1.0f;
	return material;
}

Material* Material_CreateColor(color32 color, GLfloat shininess)
{
	Material* material = (Material*)mgdl_AllocateGraphicsMemory(sizeof(Material));
	material->name = zstr_init();
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
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material->diffuseColor);
		glMaterialf(GL_FRONT, GL_SHININESS, material->shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, material->emissiveColor);
	}
}

void Material_UnApply(Material* material)
{
	if (material->texture != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}

	if( mgdl_GetLightingEnabled())
	{
		glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteSpecular);
		glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
		glMaterialfv(GL_FRONT, GL_EMISSION, blackEmissive);
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
