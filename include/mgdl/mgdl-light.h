#pragma once

#include <string>
#include "mgdl-vector.h"

enum LightType
{
	Point,
	Spot,
	Directional
};

/* Representes a light in a 3D scene.
 * Contains color and light properties
 */
struct Light
{
	vec3 color;
	float intensity = 1.0f;
	LightType type;
	float spotHalfAngle = 180.0f;
	float constantAttenuation = 1.0f;
	float LinearAttenuation = 0.0f;
	float QuadraticAttenuation = 0.0f;
	const char* name;
};
