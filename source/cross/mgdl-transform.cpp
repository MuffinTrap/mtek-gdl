#include <mgdl/mgdl-transform.h>

Transform* Transform_CreateZero()
{
	vec3 position = vec3New(0.0f, 0.0f, 0.0f);
	vec3 rotationDegrees = vec3New(0.0f, 0.0f, 0.0f);
	vec3 scale = vec3New(1.0f, 1.0f, 1.0f);
	return Transform_Create(position, rotationDegrees, scale);
}

Transform* Transform_Create(vec3 position, vec3 rotationDegrees, vec3 scale)
{
	Transform* transform = new Transform();
	transform->position = position;
	transform->rotationDegrees = rotationDegrees;
	transform->scale = scale;
	return transform;
}


void Transform_Rotate (Transform* transform, short axis, float angle )
{
	switch(axis)
	{
		case 0: V3f_X(transform->rotationDegrees) += angle; break;
		case 1: V3f_Y(transform->rotationDegrees) += angle; break;
		case 2: V3f_Z(transform->rotationDegrees) += angle; break;
	};
}

void Transform_Translate (Transform* transform, const vec3& t )
{
	V3f_X(transform->position) += V3f_X(t);
	V3f_Y(transform->position) += V3f_Y(t);
	V3f_Z(transform->position) += V3f_Z(t);
}

void Transform_SetScale3f (Transform* transform, const vec3& scale )
{
	transform->scale = scale;
}

void Transform_SetScalef (Transform* transform, float scale )
{
	transform->scale = vec3New(scale, scale, scale);
}


