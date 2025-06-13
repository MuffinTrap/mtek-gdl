#pragma once
#include <mgdl/mgdl-vector.h>
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

#ifdef __cplusplus
extern "C"
{
#endif
    /**
     * @brief Clears the rendering buffer.
     * @details Clears the rendering buffer. Use this instead of glClear directly because of bug in OpenGX
     * @param flags Flags for clearing: GL_DEPTH_BUFFER_BIT and/or GL_COLOR_BUFFER_BIT
     */
    void mgdl_glClear(GLbitfield flags);

    // NOTE: This exists just to make it easier
    // to init correctly, because Wii needs more setup than PC
    // platforms to get the same result
    void mgdl_InitPerspectiveProjection(float fovy, float nearZ, float farZ);
    void mgdl_InitOrthoProjection(void);

    // Helper functions to do common OpenGL things
    void mgdl_glSetTransparency(bool enabled);
    void mgdl_glSetAlphaTest(bool enabled);

    void mgdl_glColor4f(Color4f color);
    void mgdl_glColor4fa(Color4f color, float alpha);

    void mgdl_glVertexV3F(vec3);
    void mgdl_glVertexV3F_xy(vec3);

    void mgdl_glTriangleV3F(vec3 a, vec3 b, vec3 c);
    void mgdl_glTriangleV3F_xy(vec3 a, vec3 b, vec3 c);

/** NOTE!!!!
* @brief Sets up the camera.
* @details Without this setup the Dolphin is all messed up with the rendering.
* @note Must have gluLookAt for anything to be visible on Wii when using OpenGX.
*/
void mgdl_InitCamera(vec3 cameraPos, vec3 cameraTarget, vec3 cameraUp);

#ifdef __cplusplus
}
#endif
