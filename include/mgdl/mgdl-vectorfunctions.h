#pragma once
#include <mgdl/mgdl-vector.h>
#include <mgdl/mgdl-opengl.h>

/**
 * @file mgdl-vectorfunctions.h
 * @ingroup core
 * @brief Macro definitions for vector and matrix operations
 */


/**
 * @brief Creates a new Vector2
 * @param px X component
 * @param py Y component
 * @returns New Vector2
 */
Vector2 Vector2New(float px, float py);

/**
 * @brief Creates a new Vector3
 * @param px X component
 * @param py Y component
 * @param pz Z component
 * @returns New Vector3
 */
Vector3 Vector3New(float px, float py, float pz);
/**
 * @brief Creates a new Vector4
 * @param px X component
 * @param py Y component
 * @param pz Z component
 * @param pw W component
 * @returns New Vector4
 */
Vector4 Vector4New(float px, float py, float pz, float pw);

/**
 * @brief Rotates a direction vector around Y axis
 * @param dir Direction vector
 * @param angle Angle in radians
 * @returns Rotated vector
 */
Vector3 Vector3RotateY(Vector3 dir, float angle);
/**
 * @brief Rotates a direction vector around Z axis
 * @param dir Direction vector
 * @param angle Angle in radians
 * @returns Rotated vector
 */
Vector3 Vector3RotateZ(Vector3 dir, float angle);

/**
 * @brief Converts a 8 bit RGB color to floating point color
 * @param rx Red value between 0 - 255
 * @param gx Green value between 0 - 255
 * @param bx Blue value between 0 - 255
 * @returns Vector with normalized values for components
 */
Vector3 Vector3HexToColorFunc(int rx, int gx, int bx);

/**
 * @brief Get the value of a bezier curve at given normalized point
 * @param s The point on the bezier
 * @param P0 Point 0
 * @param C0 Control point for 0
 * @param P1 Point 1
 * @param C1 Control point for 1
 */
Vector3 Vector3SampleBezier(float s, Vector3 P0, Vector3 C0, Vector3 P1, Vector3 C1);

/**
 * @brief Constructs a matrix from OpenGL matrix
 * @param mtx Matrix from OpenGL
 * @returns New matrix with same values
 */
Matrix MatrixFromGL(GLfloat mtx[16]);


#define V3f_X(v) (v.x)

#define V3f_Y(v) (v.y)
#define V3f_Z(v) (v.z)
#define Vector3_X(v) (v.x)
#define Vector3_Y(v) (v.y)
#define Vector3_Z(v) (v.z)

#define V2f_X(v) (v.x)
#define V2f_Y(v) (v.y)
#define Vector2_X(v) (v.x)
#define Vector2_Y(v) (v.y)
