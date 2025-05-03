#pragma once

#include "mgdl-vector.h"

struct Transform
{
	vec3 position;
	vec3 rotationDegrees;
	vec3 scale;
};

extern "C"
{
	Transform* Transform_CreateZero();
	Transform* Transform_Create(vec3 position, vec3 rotationDegrees, vec3 scale);
	void Transform_Translate(Transform*, const vec3& t);
	void Transform_Rotate(Transform*, short axis, float angle);
	void Transform_SetScalef(Transform*, float scale);
	void Transform_SetScale3f(Transform*, const vec3& scale);
}
