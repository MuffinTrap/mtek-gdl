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

		gdl::vec3 position;
		gdl::vec3 rotationDegrees;
		float scale = 1.0f;
	};
}
