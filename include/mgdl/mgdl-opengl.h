#pragma once
#include <mgdl/mgdl-types.h>

// includes the OpenGL needed by platform
#ifdef GEKKO
    #include <opengx.h>
    #include <GL/glu.h>
#else
	// Needed on non-Wii platforms to silence warnings about old OpenGL
	#define GL_SILENCE_DEPRECATION 1

    #ifdef __APPLE__
        #include <OpenGL/gl.h>
        #include <OpenGL/glu.h>
        #include <GLUT/glut.h>
    #else
        #include <GL/gl.h>
        #include <GL/glu.h>
        #include <GL/glut.h>
    #endif
#endif


namespace gdl
{
    // NOTE: The reason for this might just be a bug in OpenGX
    // implementation, but it is pretty important
    void cross_glClear(GLbitfield flags);

    // NOTE: This exists just to make it easier
    // to init correctly, because Wii needs more setup than PC
    // platforms to get the same result
    void InitPerspectiveProjection(float fovy, float near, float far);
    void InitOrthoProjection();
    void InitCamera(gdl::vec3 cameraPos, gdl::vec3 cameraTarget, gdl::vec3 cameraUp);
}
