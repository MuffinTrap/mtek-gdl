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

void gdl::InitPerspectiveProjection(float fovy, float near, float far)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, gdl::GetAspectRatio(), near, far);
}

// NOTE!!!!
// Without this setup the Dolphin is
// all messed up with the
// rendering
// Must have gluLookAt for anything to be visible
// on Wii when using OpenGX
void gdl::InitCamera(gdl::vec3 cameraPos, gdl::vec3 cameraTarget, gdl::vec3 cameraUp)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z,
				 cameraTarget.x, cameraTarget.y, cameraTarget.z,
				 cameraUp.x, cameraUp.y, cameraUp.z);
}

