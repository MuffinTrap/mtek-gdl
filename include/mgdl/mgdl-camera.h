#pragma once

#include <mgdl/mgdl-vector.h>
#include <mgdl/mgdl-types.h>

struct Camera
{
	vec3 target;
	vec3 position;
	vec3 up;
	float fovY;
	float nearZ;
	float farZ;
};

Camera* Camera_CreateDefault();

void Camera_Apply(Camera* camera);
void Camera_DrawThirdsGuide(Camera* camera);
void Camera_DrawOverlayColor(Camera* camera, Color4f color, float opacity);

// TODO
/*
 * Setter functions that update necessary variables
 * Roll
 * Pitch
 * Yaw
 * SetPosition
 * SetTarget
 */
