#include <mgdl/mgdl-transform.h>

Transform Transform_CreateZero()
{
	Vector3 position = Vector3New(0.0f, 0.0f, 0.0f);
	Vector3 rotationDegrees = Vector3New(0.0f, 0.0f, 0.0f);
	Vector3 scale = Vector3New(1.0f, 1.0f, 1.0f);
	return Transform_Create(position, rotationDegrees, scale);
}

Transform Transform_Create(Vector3 position, Vector3 rotationDegrees, Vector3 scale)
{
	Transform transform;
	transform.position = position;
	transform.rotationDegrees = rotationDegrees;
	transform.scale = scale;
	return transform;
}

Transform Transform_Clone(Transform* source)
{
	return Transform_Create(source->position, source->rotationDegrees, source->scale);
}


void Transform_Rotate (Transform* transform, short axis, float angle )
{
	switch(axis)
	{
		case 0: Vector3_X(transform->rotationDegrees) += angle; break;
		case 1: Vector3_Y(transform->rotationDegrees) += angle; break;
		case 2: Vector3_Z(transform->rotationDegrees) += angle; break;
	};
}

void Transform_Translate (Transform* transform, Vector3 t )
{
	Vector3_X(transform->position) += Vector3_X(t);
	Vector3_Y(transform->position) += Vector3_Y(t);
	Vector3_Z(transform->position) += Vector3_Z(t);
}

void Transform_SetScale3f (Transform* transform, Vector3 scale )
{
	transform->scale = scale;
}

void Transform_SetScalef (Transform* transform, float scale )
{
	transform->scale = Vector3New(scale, scale, scale);
}

void Transform_Apply(Transform* transform)
{
	const Vector3 t = transform->position;
	glTranslatef(t.x, t.y, t.z);

	const Vector3 r = transform->rotationDegrees;
	glRotatef(r.x, 1.0f, 0.0f, 0.0f);
	glRotatef(r.y, 0.0f, 1.0f, 0.0f);
	glRotatef(r.z, 0.0f, 0.0f, 1.0f);

	const Vector3& s = transform->scale;
	glScalef(s.x, s.y, s.z);
}
