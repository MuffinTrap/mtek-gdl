#pragma once

#include <mgdl/mgdl-types.h>

struct Transform
{
	vec3 position;
	vec3 rotationDegrees;
	vec3 scale;
};
typedef struct Transform Transform;

#ifdef __cplusplus
extern "C"
{
#endif
	Transform* Transform_CreateZero(void);
	Transform* Transform_Create(vec3 position, vec3 rotationDegrees, vec3 scale);
	void Transform_Translate(Transform*, vec3 t);
	void Transform_Rotate(Transform*, short axis, float angle);
	void Transform_SetScalef(Transform*, float scale);
	void Transform_SetScale3f(Transform*, vec3 scale);

#ifdef __cplusplus
}
#endif
