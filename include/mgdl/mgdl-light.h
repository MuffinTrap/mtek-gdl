#pragma once

#include <string>
#include "mgdl-vector.h"

namespace gdl
{
	enum LightType
	{
		Point,
		Spot,
		Directional
	};

	/* Representes a light in a 3D scene.
	 * Contains color and light properties
	 */
	class Light
	{
	public:
		vec3 color;
		float intensity = 1.0f;
		gdl::LightType type;
		float spotHalfAngle = 180.0f;
		float constantAttenuation = 1.0f;
		float LinearAttenuation = 0.0f;
		float QuadraticAttenuation = 0.0f;
		std::string name;
	};
}
