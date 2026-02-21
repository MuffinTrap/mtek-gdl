#pragma once


/**
 * @file mgdl-light-system.h
 * @details This file contains code for the upcoming lighting system that enables
 * and disables lights based on the game situation
 */

#if 0
/**
 * @brief Structure to represent sun flares, but can be used with any light
 */
struct sun_flare_params
{
    // Camera parameters
    float camera_matrix[16];
    float camera_fov;
    float screen_aspect;

    // Flare and sun parameters
    vec3 sun_light_direction;
    vec3 sun_color;
    float sun_distance;
    float sun_radius_degrees;
    float sun_core_percentage;
    float sun_core_to_angle;
    float sun_corners;
    float sun_alpha;
    float sun_radius_to_angle;

    float lens_distance;
    float lens_fov_extra;

    float flare_amount;
    float flare_radius_degrees;
    float flare_size_adjust;
    float flare_z_step_adjust;
    float flare_z_min;
    float flare_travel_percentage;
    float flare_corners;
    float flare_alpha;
};
typedef struct sun_flare_params sun_flare_params;

void sun_flare_init_rocket(void);
void sun_flare_update_params_from_rocket(sun_flare_params* params);
void sun_flare_set_default_params(sun_flare_params* params);

void sun_flare_draw(sun_flare_params* params);
#endif
