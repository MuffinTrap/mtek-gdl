#pragma once

/**
 * @file mgdl-light.h
 * @ingroup draw3d
 * @brief Light struct and functions
 */

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

/**
 * @brief Representes a light in a 3D scene.
 * Contains color and light properties in format suitable for OpenGL
 */
struct Light
{
	float intensity;
	LightType type;
	float spotHalfAngle;
	float constantAttenuation;
	float LinearAttenuation;
	float QuadraticAttenuation;
	zstr name;

	// ufbx connection
	uint32_t ufbx_id;

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
	void Light_SetPosition(Light* light, Vector3 position);
	void Light_SetDirection(Light* light, Vector3 direction);
	void Light_SetColor(Light* light, color32 color);
	void Light_SetColorV(Light* light, Vector3 color);
	void Light_SetAmbientColor(Light* light, color32 color);
	void Light_Enable(Light* light);
	void Light_Disable(Light* light);
	void Light_Apply(Light* light);

	Vector3 Light_GetDirection(Light* light);
	void Light_LogInfo(Light* light);


#ifdef __cplusplus
}
#endif
