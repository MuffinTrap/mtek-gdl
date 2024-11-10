#include <mgdl/mgdl-transform.h>

gdl::Transform::Transform()
{
	position = gdl::vec3(0.0f, 0.0f, 0.0f);
	rotationDegrees = gdl::vec3(0.0f, 0.0f, 0.0f);
	scale = 1.0f;
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

void gdl::Transform::Translate ( const gdl::vec3& t )
{
	position.x += t.x;
	position.y += t.y;
	position.z += t.z;
}

