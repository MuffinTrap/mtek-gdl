#include <mgdl/mgdl-light.h>
#include <mgdl/mgdl-opengl.h>

Light* Light_Create()
{
    Light* l = new Light();
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
void Light_SetDirection(Light* light, vec3 direction)
{
    light->direction[0] = V3f_X(direction);
    light->direction[1] = V3f_Y(direction);
    light->direction[2] = V3f_Z(direction);
    light->direction[3] = 0.0f;
}

vec3 Light_GetDirection(Light* light)
{
    return V3f_Create(
        light->direction[0],
        light->direction[1],
        light->direction[2]);
}

void Light_SetPosition(Light* light, vec3 position)
{
    light->position[0] = V3f_X(position);
    light->position[1] = V3f_Y(position);
    light->position[2] = V3f_Z(position);
    light->position[3] = 1.0f;
}

void Light_SetColor(Light* light, Color4f* color)
{
    light->diffuse[0] = color->red;
    light->diffuse[1] = color->green;
    light->diffuse[2] = color->blue;
    light->diffuse[3] = 1.0f;
    light->specular[0] = color->red;
    light->specular[1] = color->green;
    light->specular[2] = color->blue;
    light->specular[3] = 1.0f;
}

void Light_SetAmbientColor(Light* light, Color4f* color)
{
    light->ambient[0] = color->red;
    light->ambient[1] = color->green;
    light->ambient[2] = color->blue;
    light->ambient[3] = 1.0f;
}
