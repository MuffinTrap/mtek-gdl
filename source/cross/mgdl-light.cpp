#include <mgdl/mgdl-light.h>
#include <mgdl/mgdl-opengl_util.h>

Light* Light_Create()
{
    Light* l = (Light*)malloc(sizeof(Light));
    l->type = LightType::Point;
    l->spotHalfAngle = 180.0f;
    l->constantAttenuation = 1.0f;
    l->LinearAttenuation = 0.0f;
    l->QuadraticAttenuation = 0.0f;
    l->name = nullptr;
    l->glIndex = -1;

    l->ambient[0] = 0.0f;
    l->ambient[1] = 0.0f;
    l->ambient[2] = 0.0f;
    l->ambient[3] = 1.0f;

    return l;
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
                glLightfv(GL_LIGHT0 + light->glIndex, GL_POSITION, light->direction);
            }
            // Point OR Spot
            glLightfv(GL_LIGHT0 + light->glIndex, GL_POSITION, light->position);
        }

        glLightfv(GL_LIGHT0 + light->glIndex, GL_DIFFUSE, light->diffuse);
        glLightfv(GL_LIGHT0 + light->glIndex, GL_SPECULAR, light->specular);
        glLightfv(GL_LIGHT0 + light->glIndex, GL_AMBIENT, light->ambient);
    }
}

void Light_Enable(Light* light)
{
    if (light->glIndex < 0)
    {
        light->glIndex = mgdl_EnableLightGetIndex();
    }
}

void Light_Disable(Light* light)
{
    if (light->glIndex >= 0)
    {
        mgdl_DisableLightIndex(light->glIndex );
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

void Light_SetColor(Light* light, color32 color)
{
    Color_HexToGLfloats(color, light->diffuse);;
    Color_HexToGLfloats(color, light->specular);;
}

void Light_SetAmbientColor(Light* light, color32 color)
{
    Color_HexToGLfloats(color, light->ambient);
}
