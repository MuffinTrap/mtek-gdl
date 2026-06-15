#include <mgdl/mgdl-light.h>
#include <mgdl/mgdl-opengl_util.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-console.h>

Light* Light_Create()
{
    Light* l = (Light*)malloc(sizeof(Light));
    l->type = LightType::Point;
    l->spotHalfAngle = 180.0f;
    l->constantAttenuation = 1.0f;
    l->LinearAttenuation = 0.0f;
    l->QuadraticAttenuation = 0.0f;
    l->name = zstr_init();
    l->glIndex = -1;
    l->position[0] = 0.0f;
    l->position[1] = 0.0f;
    l->position[2] = 0.0f;
    l->position[3] = 1.0f;

    l->direction[0] = 0.0f;
    l->direction[1] = 0.0f;
    l->direction[2] = 0.0f;
    l->direction[3] = 0.0f;

    l->diffuse[0] = 1.0f;
    l->diffuse[1] = 1.0f;
    l->diffuse[2] = 1.0f;
    l->diffuse[3] = 1.0f;

    l->ambient[0] = 0.0f;
    l->ambient[1] = 0.0f;
    l->ambient[2] = 0.0f;
    l->ambient[3] = 1.0f;

    return l;
}

void Light_LogInfo(Light* light)
{
    Console_SetTextColor(ConsoleText_Blue);
    Log_InfoF("Light Info\n");
    Console_ResetTextColor();
    Log_InfoF("GLIndex: %d Type %d Attenuation c%.2f l%.2f q%.2f\nColors:\n\tDiffuse:%.2f,%.2f,%.2f\n\tAmbient: %.2f,%.2f,%.2f\nPosition:%.2f,%.2f,%.2f\nDirection: %.2f,%.2f,%.2f\n",
              light->glIndex,
              (int)light->type,
              light->constantAttenuation,
              light->LinearAttenuation,
              light->QuadraticAttenuation,
              light->diffuse[0],
              light->diffuse[1],
              light->diffuse[2],
              light->ambient[0],
              light->ambient[1],
              light->ambient[2],
              light->position[0],
              light->position[1],
              light->position[2],
              light->direction[0],
              light->direction[1],
              light->direction[2]
    );
}

void Light_Apply(Light* light)
{
    if (light->glIndex >= 0)
    {
        if (light->type == LightType::Directional)
        {
            glLightfv(GL_LIGHT0 + light->glIndex, GL_POSITION, light->direction);
        }
        else
        {
            if (light->type == LightType::Spot)
            {
                glLightfv(GL_LIGHT0 + light->glIndex, GL_SPOT_DIRECTION, light->direction);
            }
            // Point OR Spot
            glLightfv(GL_LIGHT0 + light->glIndex, GL_POSITION, light->position);
        }

        glLightfv(GL_LIGHT0 + light->glIndex, GL_DIFFUSE, light->diffuse);
        glLightfv(GL_LIGHT0 + light->glIndex, GL_SPECULAR, light->specular);
        glLightfv(GL_LIGHT0 + light->glIndex, GL_AMBIENT, light->ambient);

        glEnable(GL_LIGHT0 + light->glIndex);
    }
    else
    {
        Log_Error("Cannot apply light. Light not enabled\n");
    }
}

void Light_Enable(Light* light)
{
    if (light->glIndex < 0)
    {
        light->glIndex = mgdl_ReserveLightIndex();
    }
}

void Light_Disable(Light* light)
{
    if (light->glIndex >= 0)
    {
        mgdl_FreeLightIndex(light->glIndex );
        light->glIndex = -1;
    }
}
void Light_SetDirection(Light* light, Vector3 direction)
{
    light->direction[0] = Vector3_X(direction);
    light->direction[1] = Vector3_Y(direction);
    light->direction[2] = Vector3_Z(direction);
    light->direction[3] = 0.0f;
}

Vector3 Light_GetDirection(Light* light)
{
    return Vector3New(
        light->direction[0],
        light->direction[1],
        light->direction[2]);
}

void Light_SetPosition(Light* light, Vector3 position)
{
    light->position[0] = Vector3_X(position);
    light->position[1] = Vector3_Y(position);
    light->position[2] = Vector3_Z(position);
    light->position[3] = 1.0f;
}

void Light_SetColorV(Light* light, Vector3 color)
{
    light->diffuse[0] = color.x;
    light->diffuse[1] = color.y;
    light->diffuse[2] = color.z;
    light->diffuse[3] = 1.0f;
}
void Light_SetColor(Light* light, color32 color)
{
    Color_HexToGLfloats(color, light->diffuse);;
    Color_HexToGLfloats(color, light->specular);;
}

void Light_SetAmbientColor(Light* light, color32 color)
{
    Color_HexToGLfloats(color, light->ambient);
}
