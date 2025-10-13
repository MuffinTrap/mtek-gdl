#pragma once

/**
 * @file mgdl-light.h
 * @ingroup draw3d
 * @brief Light struct and functions

#include "mgdl-types.h"
#include "mgdl-color.h"
#include "mgdl-opengl.h"

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
	float intensity;
	LightType type;
	float spotHalfAngle;
	float constantAttenuation;
	float LinearAttenuation;
	float QuadraticAttenuation;
	const char* name;

	//
	GLfloat position[4];
	GLfloat direction[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat ambient[4];

	GLint glIndex;
};

typedef struct Light Light;

#ifdef __cplusplus
extern "C"
{
#endif

	Light* Light_Create(void);
	void Light_SetPosition(Light* light, V3f position);
	void Light_SetDirection(Light* light, V3f direction);
	void Light_SetColor(Light* light, Color4f* color);
	void Light_SetAmbientColor(Light* light, Color4f* color);
	void Light_Enable(Light* light);
	void Light_Disable(Light* light);
	void Light_Apply(Light* light);

	V3f Light_GetDirection(Light* light);


#ifdef __cplusplus
}
#endif
