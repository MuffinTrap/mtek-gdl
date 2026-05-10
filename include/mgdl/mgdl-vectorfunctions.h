#pragma once
#include <mgdl/mgdl-vector.h>
#include <mgdl/mgdl-opengl.h>

/**
 * @file mgdl-vectorfunctions.h
 * @ingroup core
 * @brief Macro definitions for vector and matrix operations
 */

// NOTE
// Needed becaus ugh...
Vector2 Vector2New(float px, float py);

Vector3 Vector3New(float px, float py, float pz);
Vector4 Vector4New(float px, float py, float pz, float pw);

Vector3 Vector3RotateY(Vector3 p, float angle);
Vector3 Vector3RotateZ(Vector3 p, float angle);

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
