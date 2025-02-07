#include <mgdl/mgdl-transform.h>

gdl::Transform::Transform()
{
	position = vec3New(0.0f, 0.0f, 0.0f);
	rotationDegrees = vec3New(0.0f, 0.0f, 0.0f);
	scale = vec3New(1.0f, 1.0f, 1.0f);
}

gdl::Transform::Transform(vec3 position, vec3 rotationDegrees, vec3 scale)
{
	this->position = position;
	this->rotationDegrees = rotationDegrees;
	this->scale = scale;
}


void gdl::Transform::Rotate ( short axis, float angle )
{
	switch(axis)
	{
		case 0: rotationDegrees.x += angle; break;
		case 1: rotationDegrees.y += angle; break;
		case 2: rotationDegrees.z += angle; break;
	};
}

void gdl::Transform::Translate ( const vec3& t )
{
	position.x += t.x;
	position.y += t.y;
	position.z += t.z;
}

void gdl::Transform::SetScale3f ( const vec3& scale )
{
	this->scale = scale;
}

void gdl::Transform::SetScalef ( float scale )
{
	this->scale = vec3New(scale, scale, scale);
}


