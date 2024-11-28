#pragma once
#include <mgdl/mgdl-types.h>

namespace gdl
{
	class Transform
	{
	public:
		Transform();
		void Translate(const gdl::vec3& t);
		void Rotate(short axis, float angle);
		void SetScalef(float scale);
		void SetScale3f(const gdl::vec3& scale);

		gdl::vec3 position;
		gdl::vec3 rotationDegrees;
		gdl::vec3 scale;
	};
}
