#include <mgdl/mgdl-camera.h>
#include <mgdl/mgdl-opengl_util.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-vectorfunctions.h>

Camera* Camera_CreateDefault()
{
	Camera* camera = (Camera*)malloc(sizeof(Camera));
	camera->target = V3f_Create(0.0f, 0.0f, 0.0f);
	camera->position = V3f_Create(0.0f, 0.0f, 1.0f);
	camera->up = V3f_Create(0.0f, 1.0f, 0.0f);
	camera->fovY = 90.0f;
	camera->nearZ = 0.1f;
	camera->farZ = 1000.0f;

	return camera;
}

void Camera_Apply(Camera* camera)
{
	mgdl_InitPerspectiveProjection(camera->fovY, camera->nearZ, camera->farZ);
	switch (camera->_mode)
	{
		case CameraTarget:
			mgdl_InitCamera(camera->position, camera->target, camera->up);
		break;
		case CameraDirection:
		{
			V3f unit = V3f_Create(0.0f, 0.0f, 1.0f);
			V3f target;
			MTX3x3 transform;
			MTX3x3_Identity(transform);
			MTX3x3_RotateX(transform, Deg2Rad( V3f_X(camera->rotations)));
			MTX3x3_RotateY(transform, Deg2Rad( V3f_Y(camera->rotations)));
			MTX3x3_RotateZ(transform, Deg2Rad( V3f_Z(camera->rotations)));

			MTX3x3_MultiplyVector(transform, unit, target);
			V3f_Add(camera->position, target, camera->target);
			mgdl_InitCamera(camera->position, camera->target, camera->up);
		}
		break;

	}
}

void Camera_DrawThirdsGuide(Camera* camera)
{
	mgdl_InitOrthoProjection();

	float W = mgdl_GetScreenWidth();
	float H = mgdl_GetScreenHeight();
	float third_w = W/3.0f;
	float third_h = H/3.0f;

	glBegin(GL_LINES);
	glColor4f(0.7f, 0.7f, 0.7f, 0.5f);
	for (int w = 1; w <= 3; w++)
	{
		glVertex2f(w*third_w, H);
		glVertex2f(w*third_w, 0.0f);
	}
	for (int h = 1; h <= 3; h++)
	{
		glVertex2f(0.0f, h*third_h);
		glVertex2f(W, h*third_h);
	}
	glEnd();

	Camera_Apply(camera);
}

void Camera_DrawOverlayColor(Camera* camera, Color4f* color, float opacity)
{
	if (opacity > 0.0f)
	{
		mgdl_InitOrthoProjection();
		float W = mgdl_GetScreenWidth();
		float H = mgdl_GetScreenHeight();

        glDepthFunc(GL_ALWAYS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        mgdl_glColor4fa(color, opacity);
		glBegin(GL_QUADS);
			glVertex2f(0, 0);
			glVertex2f(W, 0);
			glVertex2f(W, H);
			glVertex2f(0, H);
		glEnd();
		glDisable(GL_BLEND);
        glDepthFunc(GL_LEQUAL);
		Camera_Apply(camera);
	}
}
void Camera_SetPositionV(Camera* camera, V3f position)
{
	camera->position = position;
}

void Camera_SetPosition(Camera* camera, float x, float y, float z)
{
	camera->position = V3f_Create(x,y,z);
}

void Camera_SetRotationsV(Camera* camera, V3f rotations)
{
	camera->rotations = rotations;
}

void Camera_SetRotations(Camera* camera, float pitch, float yaw, float roll)
{
	camera->rotations = V3f_Create(pitch, yaw, roll);
}

void Camera_SetMode(Camera* camera, CameraMode mode)
{
	camera->_mode  = mode;
}
