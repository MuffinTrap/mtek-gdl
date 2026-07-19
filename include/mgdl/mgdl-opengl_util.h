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

    /**
     * @brief Inits the perspective projection correctly on all platforms
     * @param flovy Vertical field of view
     * @param nearZ Near plane of camera
     * @param farZ Far plane of camera
     */
    void mgdl_InitPerspectiveProjection(float fovy, float nearZ, float farZ);
    /**
     * @brief Inits the orthographic projection. The current viewport is used to set the size and position
     * @param yDirection Set to positive if y increases up. Set to negative if y increases down. This makes all 2D texture and text drawing work correctly.
     */
    void mgdl_InitOrthoProjection(float yDirection);

    /**
     * @brief Returns -1 if y increases downwards and +1 if y increases up
     * @returns Y axis direction
     */
    float mgdl_glGetYDirection(void);

    /**
     * @brief Enables or disables the default transparency settings
     * @param enabled Is transparency on or off
     */
    void mgdl_glSetTransparency(bool enabled);
    /**
     * @brief Enables or disables the alpha test with default treshold
     * @param enabled Is alpha test on or off
     */
    void mgdl_glSetAlphaTest(bool enabled);
    /**
     * @brief Enables or disables the alpha test with custom treshold
     * @param enabled Is alpha test on or off
     * @param treshold What alpha values are discarded
     */
    void mgdl_glSetAlphaTestTreshold(bool enabled, float treshold);

    /**
     * @brief Sets the drawing color
     * @param color The RGBA color
     */
    void mgdl_glColor32(color32 color);
    /**
     * @brief Sets the drawing color with custom alpha
     * @param color The RGBA color
     * @param color The alpha that overrides the color's alpha
     */
    void mgdl_glColor32a(color32 color, float alpha);
    /**
     * @brief Sets the clearing color
     * @param color The RGBA color
     */
    void mgdl_glClearColor32(color32 color);

    /**
     * @brief Push a vertex
     * @param v The vertex position
     */
    void mgdl_glVertexV3F(Vector3 v);
    /**
     * @brief Push a 2D vertex
     * @param v The vertex position, z is set to 0
     */
    void mgdl_glVertexV3F_xy(Vector3 v);

    /**
     * @brief Push three vertices of a triangle
     * @param a Point A
     * @param b Point B
     * @param c Point C
     */
    void mgdl_glTriangleV3F(Vector3 a, Vector3 b, Vector3 c);
    /**
     * @brief Push three 2D vertices of a triangle. Z is set to 0
     * @param a Point A
     * @param b Point B
     * @param c Point C
     */
    void mgdl_glTriangleV3F_xy(Vector3 a, Vector3 b, Vector3 c);

    /**
     * @brief Set face culling enabled or disabled
     * @param enabled Is back face culling enabled or not
     */
    void mgdl_SetFaceCulling(bool enabled);

    /**
     * @brief Set depth testing enabled or disabled
     * @param enabled Is depth testing enabled or not
     */
    void mgdl_SetDepthTest(bool enabled);

    Vector3 mgdl_GetGLWorldForward();
    Vector3 mgdl_GetGLWorldUp();
    Vector3 mgdl_GetGLWorldRight();


    // Light status
    void mgdl_SetLightingEnabled(bool enabled);
    bool mgdl_GetLightingEnabled(void);
    GLint mgdl_ReserveLightIndex();
    void mgdl_FreeLightIndex(GLint index);
    void mgdl_SetGlobalAmbientColor32(color32 color, float strength);

/** NOTE!!!!
* @brief Sets up the camera correctly on all platforms.
* @details Without this setup the Dolphin is all messed up with the rendering.
* @note Must have gluLookAt for anything to be visible on Wii when using OpenGX.
*/
void mgdl_InitCameraV(Vector3 cameraPos, Vector3 cameraTarget, Vector3 cameraUp);
void mgdl_InitCameraF(float px, float py, float pz, float tx, float ty, float tz, float ux, float uy, float uz);

#ifdef __cplusplus
}
#endif
