#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-types.h>

// OpenGL state
static bool lightingEnabled_ = false;
static bool lightsOn_[8] = {false, false, false, false,
							false, false, false, false};

void mgdl_glClear(GLbitfield flags)
{
#ifdef GEKKO
	if ((flags & GL_DEPTH_BUFFER_BIT) != 0)
	{
		//glClearDepth(1.0f);
	}
	// NOTE
	// if GL_DEPTH_BUFFER_BIT is set, nothing is visible on Wii
	if ((flags & GL_COLOR_BUFFER_BIT) != 0)
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
#else
    glClear(flags);
#endif
}

// NOTE: variable names 'near' and 'far' are reserved on Windows
void mgdl_InitPerspectiveProjection(float fovy, float nearZ, float farZ)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, mgdl_GetAspectRatio(), nearZ, farZ);
}

void mgdl_InitOrthoProjection()
{
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    // Y increases up : OpenGL default
    gluOrtho2D(0.0, (double)mgdl_GetScreenWidth(), 0.0, (double)mgdl_GetScreenHeight());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// NOTE: This is from the OpenGL red book. The purpose is to have the vertices
	// in the middle of the screen pixels
	glTranslatef(0.375f, 0.375f, 0.0f);
}

void mgdl_InitCamera(vec3 cameraPos, vec3 cameraTarget, vec3 cameraUp)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z,
				 cameraTarget.x, cameraTarget.y, cameraTarget.z,
				 cameraUp.x, cameraUp.y, cameraUp.z);
}

void mgdl_glSetTransparency(bool enabled)
{
	if (enabled)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

void mgdl_glSetAlphaTest(bool enabled)
{
	if (enabled)
	{
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GEQUAL, 0.5f);
	}
	else
	{
		glDisable(GL_ALPHA_TEST);
		glAlphaFunc(GL_ALWAYS, 0.5f);
	}
}

void mgdl_SetFaceCulling(bool enabled)
{
	if (enabled)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

void mgdl_SetDepthTest(bool enabled)
{
	if (enabled)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}

void mgdl_SetLightingEnabled(bool enabled)
{
	if (enabled == lightingEnabled_)
	{
		return;
	}
	if (enabled)
	{
		glEnable(GL_LIGHTING);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}
	lightingEnabled_ = enabled;
}

bool mgdl_GetLightingEnabled(void)
{
	return lightingEnabled_;
}

GLint mgdl_EnableLightGetIndex(void)
{
	for (GLint i = 0; i < 8; i++)
	{
		if (lightsOn_[i] == false)
		{
			lightsOn_[i] = true;
			glEnable(GL_LIGHT0 + i);
			return i;
		}
	}
	return -1;
}

void mgdl_DisableLightIndex(GLint index)
{
	if (index >=0 && index < 8)
	{
		lightsOn_[index] = false;
		glDisable(GL_LIGHT0 + index);
	}
}

void mgdl_glColor4f(Color4f color)
{
	glColor4f(color.red, color.green, color.blue, color.alpha);
}
void mgdl_glColor4fa(Color4f color, float alpha)
{
	glColor4f(color.red, color.green, color.blue, alpha);
}

void mgdl_glVertexV3F(vec3 position)
{
	glVertex3f(V3f_X(position), V3f_Y(position), V3f_Z(position));
}

void mgdl_glVertexV3F_xy(vec3 position)
{
	glVertex2f(V3f_X(position), V3f_Y(position));
}

void mgdl_glTriangleV3F_xy(vec3 a, vec3 b, vec3 c)
{
	mgdl_glVertexV3F_xy(a);
	mgdl_glVertexV3F_xy(b);
	mgdl_glVertexV3F_xy(c);
}

void mgdl_glTriangleV3F(vec3 a, vec3 b, vec3 c)
{
	mgdl_glVertexV3F(a);
	mgdl_glVertexV3F(b);
	mgdl_glVertexV3F(c);
}

