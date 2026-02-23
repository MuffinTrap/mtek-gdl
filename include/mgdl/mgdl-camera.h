#pragma once

/**
 * @file mgdl-camera.h
 * @ingroup draw3d
 * @brief Camera class
 */

#include <mgdl/mgdl-vector.h>
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-color.h>

struct Camera
{
	V3f target;
	V3f position;
	V3f up;
	V3f rotations;
	V3f direction;

	float fovY;
	float nearZ;
	float farZ;

	CameraMode mode;
	CameraProjection projection;
};
typedef struct Camera Camera;


#ifdef __cplusplus
extern "C"
{
#endif


Camera* Camera_CreateDefault(void);

void Camera_Apply(Camera* camera);
void Camera_DrawThirdsGuide(Camera* camera);
void Camera_DrawOverlayColor(Camera* camera, Color4f* color, float opacity);

void Camera_SetPositionV(Camera* camera, V3f position);
void Camera_SetPosition(Camera* camera, float x, float y, float z);
void Camera_SetRotationsV(Camera* camera, V3f rotations);
void Camera_SetRotations(Camera* camera, float pitch, float yaw, float roll);
void Camera_SetDirection(Camera* camera, vec3 direction);

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
