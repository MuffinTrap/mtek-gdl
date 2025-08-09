#include <mgdl/mgdl-transform.h>

Transform* Transform_CreateZero()
{
	V3f position = V3f_Create(0.0f, 0.0f, 0.0f);
	V3f rotationDegrees = V3f_Create(0.0f, 0.0f, 0.0f);
	V3f scale = V3f_Create(1.0f, 1.0f, 1.0f);
	return Transform_Create(position, rotationDegrees, scale);
}

Transform* Transform_Create(V3f position, V3f rotationDegrees, V3f scale)
{
	Transform* transform = (Transform*)malloc(sizeof(Transform));
	transform->position = position;
	transform->rotationDegrees = rotationDegrees;
	transform->scale = scale;
	return transform;
}

Transform* Transform_Clone(Transform* source)
{
	return Transform_Create(source->position, source->rotationDegrees, source->scale);
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

void Transform_Translate (Transform* transform, V3f t )
{
	V3f_X(transform->position) += V3f_X(t);
	V3f_Y(transform->position) += V3f_Y(t);
	V3f_Z(transform->position) += V3f_Z(t);
}

void Transform_SetScale3f (Transform* transform, V3f scale )
{
	transform->scale = scale;
}

void Transform_SetScalef (Transform* transform, float scale )
{
	transform->scale = V3f_Create(scale, scale, scale);
}


