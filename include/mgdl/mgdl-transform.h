#pragma once
#include <mgdl/mgdl-types.h>

namespace gdl
{
	class Transform
	{
	public:
		gdl::vec3 position;
		gdl::vec3 rotationDegrees;
		float scale = 1.0f;
	};
}
