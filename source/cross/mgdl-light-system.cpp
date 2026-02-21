// TODO Update to mgdl
#if 0
#include "sun_flare.h"

#include "../util/screenprint.h"

#include "m_math_vector_macros.h"

// NOTE for debugging
static void draw_cross(vec3 position, vec3 right, vec3 up, float size)
{
    glBegin(GL_LINES);
    // Right
    glVertex3f( V3f_X(position), V3f_Y(position), V3f_Z(position));
    glVertex3f( V3f_X(position) + V3f_X(right) * size,
                V3f_Y(position) + V3f_Y(right) * size,
                V3f_Z(position) + V3f_Z(right) * size );

    // left
    glVertex3f( V3f_X(position), V3f_Y(position), V3f_Z(position));
    glVertex3f( V3f_X(position) - V3f_X(right) * size,
                V3f_Y(position) - V3f_Y(right) * size,
                V3f_Z(position) - V3f_Z(right) * size );

    // up
    glVertex3f( V3f_X(position), V3f_Y(position), V3f_Z(position));
    glVertex3f( V3f_X(position) + V3f_X(up) * size,
                V3f_Y(position) + V3f_Y(up) * size,
                V3f_Z(position) + V3f_Z(up) * size );

    // left
    glVertex3f( V3f_X(position), V3f_Y(position), V3f_Z(position));
    glVertex3f( V3f_X(position) - V3f_X(up) * size,
                V3f_Y(position) - V3f_Y(up) * size,
                V3f_Z(position) - V3f_Z(up) * size );

    glEnd();

}

void sun_flare_init_rocket(void)
{
    // TODO
}
void sun_flare_update_params_from_rocket(sun_flare_params* params)
{
    // TODO
}


void sun_flare_set_default_params(sun_flare_params* params)
{
    // Default values
    params->sun_light_direction = V3f_Create(0.0f, 0.0f, 1.0f); /**< Direction of light */
    params->sun_color = V3f_Create(1.0f, 1.0f, 0.8f);
    params->sun_distance = 200.0f;  /**< How far the sun is from origo */
    params->sun_radius_degrees = 8.0f;  /**< How many degrees the sun radius is */
    params->sun_core_percentage = 0.05f;  /**< How much of the sun is solid color */
    params->sun_core_to_angle = 0.5f;  /**< How much that changes depending on camera to sun angle */
    params->sun_corners = 6.0f; /**< How many corners, what N-gon the  sun is */
    params->sun_alpha = 0.0f;  /**< What is the alpha on the border of the sun */
    params->lens_distance = 20.0f; /**< How far from origo the flares are drawn */
    params->lens_fov_extra = 15.f; /**< How many degrees the sun can be outside view and still be drawn*/

    params->flare_amount = 4.0f; /**< How many flares are drawn */
    params->flare_radius_degrees = 1.5f;  /**< Radius of a flare in degrees at sun distance */
    params->flare_size_adjust = 0.9f; /**< How much the radius changes as the flare comes closer to camera*/
    params->flare_z_step_adjust = 0.6f; /**< Adjust how bunched up the flares are when angle to sun increases */
    params->flare_z_min = -1.0f; /**< How much the flares are allowed to come closer than lens_distance*/
    params->flare_corners = 5.0f; /**< Shape of a flare, what N-gon */
    params->flare_alpha = 0.2f; /**< Alpha at the flare border */
    params->flare_travel_percentage = 0.4f; /**< To what percentage of distance from lens_distance to sun_distance the flares are drawn. Small values put the flares closer together */
}

static inline void vertex(vec3* center, vec3* offset, float scale)
{
        glVertex3f( V3f_X(*center) + V3f_X(*offset) * scale,
                    V3f_Y(*center) + V3f_Y(*offset) * scale,
                    V3f_Z(*center) + V3f_Z(*offset) * scale );
}


void sun_flare_draw(sun_flare_params* params)
{
    // World axis vectors
    vec3 w_right = V3f_Create(1.0f, 0.0f, 0.0f);
    vec3 w_up = V3f_Create(0.0f, 1.0f, 0.0f);
    vec3 w_forward = V3f_Create(0.0f, 0.0f, 1.0f);
    vec3 zero = V3f_Create(0.0f, 0.0f, 0.0f);

    // Camera local axis vectors
    vec3 camera_right;
    vec3 camera_up;
    vec3 camera_forward;

    // Camera position in world
    vec3 camera_pos;

    // Euler angles of camera
    vec3 camera_angles;

    // Extract camera vectors from matrix
    m_mat4_rotate3(&camera_right, params->camera_matrix, &w_right);
    m_mat4_rotate3(&camera_up, params->camera_matrix, &w_up);
    m_mat4_rotate3(&camera_forward, params->camera_matrix, &w_forward);
    m_mat4_transform3(&camera_pos, params->camera_matrix, &zero);
    m_mat4_extract_euler(&camera_angles, params->camera_matrix);

    const float camera_yaw = V3f_Y(camera_angles);
    const float camera_elevation = V3f_X(camera_angles);

    vec3 to_light; V3f_Negate(params->sun_light_direction, to_light);
    float dot = V3f_Dot(camera_forward, to_light);

    float fov_rad = M_DEG_TO_RAD * params->camera_fov;
    float fov_rad_pitch = fov_rad / 2.0f; // Half angle of camera
    float fov_rad_yaw = fov_rad_pitch * params->screen_aspect;

    float angle_to_sun = acosf(dot);
    float angle_percentage = angle_to_sun / (fov_rad_yaw);
    screenprintf("Angle %: %.2f", angle_percentage);

    if ((angle_to_sun-params->lens_fov_extra * M_DEG_TO_RAD) > fov_rad_pitch && angle_to_sun > fov_rad_yaw)
    {
        // Sun is outside of field of view
       return;
    }

    screenprintf("Camera dir: %.2f, %.2f, %.2f", camera_forward.x, camera_forward.y, camera_forward.z);
    screenprintf("Camera fov: %.2f Angle to sun %.2f", fov_rad, angle_to_sun);
    screenprintf("Sun dir: %.2f, %.2f, %.2f", params->sun_light_direction.x, params->sun_light_direction.y, params->sun_light_direction.z);
    screenprintf("To sun: %.2f, %.2f, %.2f", to_light.x, to_light.y, to_light.z);
    screenprintf("Camera-Sun %.2f\n", dot);

    vec3 sun_position;
    V3f_Scale(to_light, params->sun_distance - 0.01f, sun_position);

    vec3 sun_forward;
    vec3 sun_right;
    vec3 sun_up;
    V3f_Copy(camera_forward, sun_forward);
    V3f_Copy(camera_right, sun_right);
    V3f_Copy(camera_up, sun_up);

    vec3 flare_target;
    V3f_Scale(camera_forward, params->lens_distance, flare_target);
    V3f_Add(camera_pos, flare_target, flare_target);
    screenprintf("Flare target: %.2f, %.2f, %.2f", flare_target.x, flare_target.y, flare_target.z);

    float rotation_mtx[16];
    m_mat4_identity(rotation_mtx);
    m_mat4_rotation_axis(rotation_mtx, &camera_up, params->sun_radius_degrees * M_DEG_TO_RAD);
    vec3 sun_border;
    m_mat4_rotate3(&sun_border, rotation_mtx, &sun_position);
    vec3 sun_radius_vec;
    V3f_Sub(sun_position, sun_border, sun_radius_vec);
    float sun_draw_radius = V3f_Magnitude(sun_radius_vec);

    // Draw sun and flares
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // NOTE DRAW SUN

    vec3 sun_color;
    V3f_Copy(params->sun_color, sun_color);

    float sun_core_radius = sun_draw_radius * params->sun_core_percentage;
    float rest_of_radius = sun_draw_radius * (1.0f-params->sun_core_percentage);
    sun_core_radius += rest_of_radius * (1.0f - angle_percentage) * params->sun_core_to_angle;
    if (sun_core_radius < 0.0f) { sun_core_radius = 0.0f;}
    float sun_draw_alpha = params->sun_alpha;

    vec3 sun_point;
    float sun_angle_step = M_PI * 2.0f / params->sun_corners;

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(sun_color.x, sun_color.y, sun_color.z, 1.0f);
    glVertex3f( V3f_X(sun_position), V3f_Y(sun_position), V3f_Z(sun_position));
    for (int s = 0; s < (int)params->sun_corners+1; s++)
    {
        m_mat4_identity(rotation_mtx);
        m_mat4_rotation_axis(rotation_mtx, &camera_forward, sun_angle_step * -s);
        m_mat4_rotate3(&sun_point, rotation_mtx, &camera_up);

        vertex(&sun_position, &sun_point, sun_core_radius);
    }
    glEnd();
    glBegin(GL_QUAD_STRIP);
    for (int s = 0; s < (int)params->sun_corners+1; s++)
    {
        m_mat4_identity(rotation_mtx);
        m_mat4_rotation_axis(rotation_mtx, &camera_forward, sun_angle_step * -s);
        m_mat4_rotate3(&sun_point, rotation_mtx, &camera_up);

        glColor4f(sun_color.x, sun_color.y, sun_color.z, 1.0f);
        vertex(&sun_position, &sun_point, sun_core_radius);

        glColor4f(sun_color.x, sun_color.y, sun_color.z, sun_draw_alpha);
        vertex(&sun_position, &sun_point, sun_draw_radius); // v0
    }
    glEnd();

    // NOTE DRAW FLARES

    m_mat4_identity(rotation_mtx);
    m_mat4_rotation_axis(rotation_mtx, &camera_up, params->flare_radius_degrees * M_DEG_TO_RAD);
    vec3 flare_border;
    m_mat4_rotate3(&flare_border, rotation_mtx, &sun_position);
    vec3 flare_radius_vec;
    V3f_Sub(sun_position, flare_border, flare_radius_vec);
    float flare_draw_radius = V3f_Magnitude(flare_radius_vec);

    vec3 flare_point; // polygon points
    vec3 flare_position; // center of flare

    vec3 flare_dir;
    vec3 flare_dir_normal;
    V3f_Sub(sun_position, flare_target, flare_dir);
    V3f_Normalize(flare_dir, flare_dir_normal);

    float dot_exp = angle_percentage;
    float flare_radius = flare_draw_radius;
    float flaregon = params->flare_corners;

    float angle_step = M_PI * 2.0f / flaregon;
    float start_angle_step =  M_PI / (params->flare_amount);

    float flare_travel = V3f_Magnitude(flare_dir);
    float flare_z_max = flare_travel * params->flare_travel_percentage;
    float z_step = flare_travel/params->flare_amount;

    // Place the first flare closer to flare target when angle to sun is bigger
    float move = (1.0f-angle_percentage);
    if (move < params->flare_z_min)
    {
        move = params->flare_z_min;
    }

    float flare_z_min = move * z_step;

    // Draw flares towards camera
    int flares = (int)params->flare_amount;

    for (int i = 0; i < flares; i++)
    {
        int index = (flares-(i+1));
        m_mat4_identity(rotation_mtx);
        V3f_Scale(flare_dir_normal, flare_z_min + (params->flare_z_step_adjust*z_step) * index, flare_position);
        V3f_Add(flare_target, flare_position, flare_position);

        // Center
        glColor4f(sun_color.x, sun_color.y, sun_color.z, 0.0f);

        // Corners
        glBegin(GL_TRIANGLE_FAN);
        vertex(&flare_position, &zero, 0.0f);
        for (int g = 0; g < flaregon+1; g++)
        {
            m_mat4_rotation_axis(rotation_mtx, &camera_forward, (angle_step * -g + start_angle_step * i));
            m_mat4_rotate3(&flare_point, rotation_mtx, &camera_up);
            float scale = flare_radius * params->flare_size_adjust;
            glColor4f(sun_color.x, sun_color.y, sun_color.z, params->flare_alpha * angle_percentage);
            vertex(&flare_position, &flare_point, scale);
        }
        glEnd();
    }

    glDisable(GL_BLEND);
}
#endif
