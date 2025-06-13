#include <mgdl/mgdl-light.h>

Light* Light_Create()
{
    Light* l = new Light();
    l->color = V3f_Create(1.0f, 1.0f, 1.0f);
    l->type = LightType::Point;
    l->spotHalfAngle = 180.0f;
    l->constantAttenuation = 1.0f;
    l->LinearAttenuation = 0.0f;
    l->QuadraticAttenuation = 0.0f;
    l->name = nullptr;
    return l;
}
