#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-main.h>

void gdl::cross_glClear(GLbitfield flags)
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
void gdl::InitPerspectiveProjection(float fovy, float nearZ, float farZ)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, gdl::GetAspectRatio(), nearZ, farZ);
}

void gdl::InitOrthoProjection()
{
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    // Y increases up : OpenGL default
    gluOrtho2D(0.0, (double)gdl::GetScreenWidth(), 0.0, (double)gdl::GetScreenHeight());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// NOTE: This is from the OpenGL red book. The purpose is to have the vertices
	// in the middle of the screen pixels
	glTranslatef(0.375f, 0.375f, 0.0f);
}


// NOTE!!!!
// Without this setup the Dolphin is
// all messed up with the
// rendering
// Must have gluLookAt for anything to be visible
// on Wii when using OpenGX
void gdl::InitCamera(vec3 cameraPos, vec3 cameraTarget, vec3 cameraUp)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z,
				 cameraTarget.x, cameraTarget.y, cameraTarget.z,
				 cameraUp.x, cameraUp.y, cameraUp.z);
}

