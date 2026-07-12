#include <mgdl/mgdl-opengl_util.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-types.h>

// OpenGL state
static bool lightingEnabled_ = false;
static bool lightsOn_[8] = {false, false, false, false,
							false, false, false, false};
static GLfloat globalAmbient[4] = {0.0f, 0.0f, 0.0f, 1.0f};

static const Vector3 FORWARD = Vector3New(0, 0, -1);
static const Vector3 UP = Vector3New(0, 1, 0);
static const Vector3 RIGHT = Vector3New(1, 0, 0);

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
	Viewport viewport = mgdl_GetViewport();
    gluOrtho2D(0.0, (double)viewport.width, 0.0, (double)viewport.height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// NOTE: This is from the OpenGL red book. The purpose is to have the vertices
	// in the middle of the screen pixels
	glTranslatef(0.375f, 0.375f, 0.0f);
}

void mgdl_InitCameraF(float px, float py, float pz, float tx, float ty, float tz, float ux, float uy, float uz)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	gluLookAt(px, py, pz,
				 tx, ty, tz,
				 ux, uy, uz);

}

void mgdl_InitCameraV(Vector3 cameraPos, Vector3 cameraTarget, Vector3 cameraUp)
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
	mgdl_glSetAlphaTestTreshold(enabled, 0.5f);
}
void mgdl_glSetAlphaTestTreshold(bool enabled, float treshold)
{
	if (enabled)
	{
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GEQUAL, treshold);
	}
	else
	{
		glDisable(GL_ALPHA_TEST);
		glAlphaFunc(GL_ALWAYS, treshold);
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

GLint mgdl_ReserveLightIndex(void)
{
	for (GLint i = 0; i < 8; i++)
	{
		if (lightsOn_[i] == false)
		{
			lightsOn_[i] = true;
			return i;
		}
	}
	return -1;
}

void mgdl_FreeLightIndex(GLint index)
{
	if (index >=0 && index < 8)
	{
		lightsOn_[index] = false;
	}
}
void mgdl_SetGlobalAmbientColor32(color32 color, float strength)
{
	RGBAf ambient = Color_HexToFloats(color);
	globalAmbient[0]=ambient.red * strength;
	globalAmbient[1]=ambient.green * strength;
	globalAmbient[2]=ambient.blue * strength;
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
}

void mgdl_glColor3f(color32 color)
{
	glColor3f(REDf(color), GREENf(color), BLUEf(color));
}
void mgdl_glColor32(color32 color)
{
	glColor4f(REDf(color), GREENf(color), BLUEf(color), ALPHAf(color));
}
void mgdl_glColor32a(color32 color, float alpha)
{
	glColor4f(REDf(color), GREENf(color), BLUEf(color), alpha);
}

void mgdl_glClearColor32(color32 color)
{
	glClearColor(REDf(color), GREENf(color), BLUEf(color), ALPHAf(color));
}

void mgdl_glVertexV3F(Vector3 position)
{
	glVertex3f(Vector3_X(position), Vector3_Y(position), Vector3_Z(position));
}

void mgdl_glVertexV3F_xy(Vector3 position)
{
	glVertex2f(Vector3_X(position), Vector3_Y(position));
}

void mgdl_glTriangleV3F_xy(Vector3 a, Vector3 b, Vector3 c)
{
	mgdl_glVertexV3F_xy(a);
	mgdl_glVertexV3F_xy(b);
	mgdl_glVertexV3F_xy(c);
}

void mgdl_glTriangleV3F(Vector3 a, Vector3 b, Vector3 c)
{
	mgdl_glVertexV3F(a);
	mgdl_glVertexV3F(b);
	mgdl_glVertexV3F(c);
}

Vector3 mgdl_GetGLWorldForward()
{
	return FORWARD;
}
Vector3 mgdl_GetGLWorldUp()
{
	return UP;
}
Vector3 mgdl_GetGLWorldRight()
{
	return RIGHT;
}

