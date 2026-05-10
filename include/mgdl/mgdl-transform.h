#pragma once

#include <mgdl/mgdl-types.h>

/**
 * @file mgdl-transform.h
 * @ingroup scene
 * @brief Transform class for scene graph
 */

struct Transform
{
	Vector3 position;
	Vector3 rotationDegrees;
	Vector3 scale;
};
typedef struct Transform Transform;

#ifdef __cplusplus
extern "C"
{
#endif
	Transform* Transform_CreateZero(void);
	Transform* Transform_Create(Vector3 position, Vector3 rotationDegrees, Vector3 scale);
	Transform* Transform_Clone(Transform* source);
	void Transform_Translate(Transform*, Vector3 t);
	void Transform_Rotate(Transform*, short axis, float angle);
	void Transform_SetScalef(Transform*, float scale);
	void Transform_SetScale3f(Transform*, Vector3 scale);

#ifdef __cplusplus
}
#endif
