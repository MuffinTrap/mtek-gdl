#pragma once

#include <mgdl/mgdl-vector.h>
#include <mgdl/mgdl-types.h>

struct Camera
{
	vec3 target;
	vec3 position;
	vec3 up;
	vec3 rotations;

	float fovY;
	float nearZ;
	float farZ;

	CameraMode _mode;
};
typedef struct Camera Camera;


#ifdef __cplusplus
extern "C"
{
#endif


Camera* Camera_CreateDefault(void);

void Camera_Apply(Camera* camera);
void Camera_DrawThirdsGuide(Camera* camera);
void Camera_DrawOverlayColor(Camera* camera, Color4f color, float opacity);

void Camera_SetPositionV(Camera* camera, vec3 position);
void Camera_SetPosition(Camera* camera, float x, float y, float z);
void Camera_SetRotationsV(Camera* camera, vec3 rotations);
void Camera_SetRotations(Camera* camera, float pitch, float yaw, float roll);

void Camera_SetMode(Camera* camera, CameraMode mode);

#ifdef __cplusplus
}
#endif

// TODO
/*
 * Setter functions that update necessary variables
 * Roll
 * Pitch
 * Yaw
 * SetPosition
 * SetTarget
 */
