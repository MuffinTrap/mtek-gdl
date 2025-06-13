#pragma once

#include "mgdl-types.h"

enum LightType
{
	Point,
	Spot,
	Directional
};
typedef enum LightType LightType;

/* Representes a light in a 3D scene.
 * Contains color and light properties
 */
struct Light
{
	vec3 color;
	float intensity;
	LightType type;
	float spotHalfAngle;
	float constantAttenuation;
	float LinearAttenuation;
	float QuadraticAttenuation;
	const char* name;
};

typedef struct Light Light;

#ifdef __cplusplus
extern "C"
{
#endif

	Light* Light_Create(void);


#ifdef __cplusplus
}
#endif
