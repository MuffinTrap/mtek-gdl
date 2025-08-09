#pragma once

#include <mgdl/mgdl-types.h>

struct Transform
{
	V3f position;
	V3f rotationDegrees;
	V3f scale;
};
typedef struct Transform Transform;

#ifdef __cplusplus
extern "C"
{
#endif
	Transform* Transform_CreateZero(void);
	Transform* Transform_Create(V3f position, V3f rotationDegrees, V3f scale);
	Transform* Transform_Clone(Transform* source);
	void Transform_Translate(Transform*, V3f t);
	void Transform_Rotate(Transform*, short axis, float angle);
	void Transform_SetScalef(Transform*, float scale);
	void Transform_SetScale3f(Transform*, V3f scale);

#ifdef __cplusplus
}
#endif
