#pragma once

#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-vector.h>
#include <mgdl/mgdl-color.h>

/**
 * @file mgdl-opengl_util.h
 * @ingroup draw2d draw3d
 * @brief Utility functions to make using OpenGL easier
 */

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

    void mgdl_glColor3f(Color4f* color);
    void mgdl_glColor4f(Color4f* color);
    void mgdl_glColor4fa(Color4f* color, float alpha);
    void mgdl_glClearColor4f(Color4f* color);

    void mgdl_glVertexV3F(V3f);
    void mgdl_glVertexV3F_xy(V3f);

    void mgdl_glTriangleV3F(V3f a, V3f b, V3f c);
    void mgdl_glTriangleV3F_xy(V3f a, V3f b, V3f c);

    void mgdl_SetFaceCulling(bool enabled);

    void mgdl_SetDepthTest(bool enabled);


    // Light status
    void mgdl_SetLightingEnabled(bool enabled);
    bool mgdl_GetLightingEnabled(void);
    GLint mgdl_EnableLightGetIndex(void);
    void mgdl_DisableLightIndex(GLint index);

/** NOTE!!!!
* @brief Sets up the camera.
* @details Without this setup the Dolphin is all messed up with the rendering.
* @note Must have gluLookAt for anything to be visible on Wii when using OpenGX.
*/
void mgdl_InitCamera(V3f cameraPos, V3f cameraTarget, V3f cameraUp);

#ifdef __cplusplus
}
#endif
