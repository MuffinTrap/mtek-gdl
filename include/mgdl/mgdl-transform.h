#pragma once

#include "mgdl-vector.h"

namespace gdl
{
	class Transform
	{
	public:
		Transform();
		Transform(vec3 position, vec3 rotationDegrees, vec3 scale);
		void Translate(const vec3& t);
		void Rotate(short axis, float angle);
		void SetScalef(float scale);
		void SetScale3f(const vec3& scale);

		vec3 position;
		vec3 rotationDegrees;
		vec3 scale;
	};
}
