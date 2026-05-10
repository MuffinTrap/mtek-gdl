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
    void mgdl_glSetAlphaTestTreshold(bool enabled, float treshold);

    void mgdl_glColor32(color32 color);
    void mgdl_glColor32a(color32 color, float alpha);
    void mgdl_glClearColor32(color32 color);

    void mgdl_glVertexV3F(Vector3);
    void mgdl_glVertexV3F_xy(Vector3);

    void mgdl_glTriangleV3F(Vector3 a, Vector3 b, Vector3 c);
    void mgdl_glTriangleV3F_xy(Vector3 a, Vector3 b, Vector3 c);

    void mgdl_SetFaceCulling(bool enabled);

    void mgdl_SetDepthTest(bool enabled);

    Vector3 mgdl_GetGLWorldForward();
    Vector3 mgdl_GetGLWorldUp();
    Vector3 mgdl_GetGLWorldRight();


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
void mgdl_InitCamera(Vector3 cameraPos, Vector3 cameraTarget, Vector3 cameraUp);

#ifdef __cplusplus
}
#endif
