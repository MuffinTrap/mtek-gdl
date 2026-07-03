#pragma once

#include <mgdl/mgdl-types.h>

/**
 * @file mgdl-transform.h
 * @ingroup scene
 * @brief Transform class for scene graph
 */

/**
 * @brief Transform to help setting the OpenGL modelview matrix
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
	Transform Transform_CreateZero(void);
	Transform Transform_Create(Vector3 position, Vector3 rotationDegrees, Vector3 scale);
	Transform Transform_Clone(Transform* source);
	void Transform_Translate(Transform* transform, Vector3 translation);
	void Transform_Rotate(Transform* transform, short axis, float angle);
	void Transform_SetScalef(Transform* transform, float scale);
	void Transform_SetScale3f(Transform* transform, Vector3 scale);

	/**
	 * @brief Applies a transform to OpenGL ModelViewMatrix
	 * @param transform The Transform to apply
	 */
	void Transform_Apply(Transform* transform);

#ifdef __cplusplus
}
#endif
